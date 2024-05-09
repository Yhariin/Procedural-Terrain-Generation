#include "TerrainGenerator.hpp"
#include <iostream>
#include <random>
#include <cstdlib>
#include <chrono>

// Timer class for benchmarking performance, can be moved to a different file.
struct Timer
{
    std::chrono::time_point<std::chrono::steady_clock> start, end;
    std::chrono::duration<float> duration;

    Timer()
    {
        start = std::chrono::high_resolution_clock::now();
    }

    void End()
    {
        end = std::chrono::high_resolution_clock::now();
        duration = end - start;

    }
};

// Currently there is hitching when loading terrain due to the TerrainGenerator being
// forced to generate the terrain in the constructor. To prevent hitching we should 
// move the all the generation onto its own method and leave the constructor for 
// initializing variables. That way we can move the creation of the generation onto
// its own thread. The reason why we cant do this now is because the TerrainGenerator
// is constructed in the Renderer using list initialization, and from what I know, there
// is no way to move that onto a different thread.
TerrainGenerator::TerrainGenerator(TerrainProperties &terrainProperties) 
{
    // Timer normalTimer;
    // GenerateChunk(terrainProperties.resolution, terrainProperties);
    // normalTimer.End();
    // std::cout << "normal: " << normalTimer.duration.count() << "s " << std::endl;

    m_IndexCounter = 0;
    Timer threadedTimer;
    GenerateChunkThreaded(terrainProperties.resolution, terrainProperties);
    threadedTimer.End();
    std::cout << "threaded: " << threadedTimer.duration.count() << "s " << std::endl;
    std::cout << "---------------------------------------------------------" << std::endl;

    computeVertexProperties(m_VertexProperties, m_Vertices);
    m_VertexBufferLayout.Push<float>(m_VertexProperties.size_pos);
    m_VertexBufferLayout.Push<float>(m_VertexProperties.size_color);
    m_VertexBufferLayout.Push<float>(m_VertexProperties.size_normal);
}

TerrainGenerator::~TerrainGenerator()
{

}

void TerrainGenerator::computeVertexProperties(VertexProperties &vertexProperties, std::vector<float> &vertices)
{
    vertexProperties.obj = vertices;
    vertexProperties.obj_count = vertices.size();
    // vertexProperties.obj_count = sizeof(vertices_t) / sizeof(float);

    vertexProperties.size_pos = 3;
    vertexProperties.offset_pos = 0;

    vertexProperties.size_color = 3;
    vertexProperties.offset_color = vertexProperties.size_pos * sizeof(float);

    vertexProperties.size_normal = 3;
    vertexProperties.offset_normal = vertexProperties.offset_color + (vertexProperties.size_color * sizeof(float));

    vertexProperties.stride = (vertexProperties.size_pos + vertexProperties.size_color) * 4;
}

void TerrainGenerator::GenerateVertices(int threadNumber, uint16_t NUM_THREADS, int resolution, TerrainProperties &terrainProperties, std::vector<glm::vec2> &octaveOffsets)
{
    const unsigned int load = resolution / NUM_THREADS;
    float spacing = 1.f / (float(resolution) / 64.f);
    int left = resolution - (load * NUM_THREADS);
    int upperBound = (threadNumber == NUM_THREADS-1) ? (threadNumber*load)+load+left : (threadNumber*load)+load;

    for(int r = threadNumber*load; r < upperBound; ++r)
    {
        for(int c = 0; c < resolution; ++c)
        {
            float x = c;
            float z = r;

            float xs = x / terrainProperties.scale;
            float zs = z / terrainProperties.scale;

            float amplitude = 1;
            float frequency = 1;
            float noiseHeight = 0;

            float perlinValue;
            for(int i = 0; i < terrainProperties.octaves; ++i)
            {
                float sampleX = xs * frequency + octaveOffsets[i].x;
                float sampleZ = zs * frequency + octaveOffsets[i].y;

                switch (terrainProperties.noiseType)
                {
                case 0:
                    perlinValue = glm::perlin(glm::vec2(sampleX, sampleZ));
                    break;
                case 1:
                    perlinValue = glm::simplex(glm::vec2(sampleX, sampleZ));
                    break;
                }
                
                noiseHeight += perlinValue * amplitude;

                // amplitude *= terrainProperties.persistance;
                amplitude = pow(terrainProperties.persistance, i);
                frequency = pow(terrainProperties.lacunarity, i);
                // frequency *= terrainProperties.lacunarity;

            }
            // xOff += terrainProperties.scale;

            noiseHeight *= terrainProperties.height;

            if(noiseHeight < terrainProperties.floor && terrainProperties.floor != 0.f)
                noiseHeight = terrainProperties.floor;

            float y = noiseHeight;

            glm::vec3 v(x * spacing, y * spacing, z * spacing);

            std::vector<int> triangles;

            m_Mutex.lock();

            m_VerticesVec3[m_threadIndexVec[threadNumber]] = v;
            m_threadIndexVec[threadNumber] += 1;
            m_IndexCounter++;

            m_VertexColorsVec3.push_back(terrainProperties.colorVec);

            // Create a vector in the mapping
            // To keep track of all the triangles
            m_VertexToTrianglesMap.emplace(m_IndexCounter-1, triangles);

            m_Mutex.unlock();
        }

    }

}

void TerrainGenerator::GenerateChunkThreaded(int resolution, TerrainProperties &terrainProperties)
{
    srand(terrainProperties.seed);
    std::mt19937 gen(terrainProperties.seed);
    std::uniform_int_distribution<> distr(-10000, 10000);

    std::vector<glm::vec2> octaveOffsets;
    octaveOffsets.reserve(terrainProperties.octaves);
    for(int i = 0; i < terrainProperties.octaves; ++i)
    {
        float offsetX = distr(gen);
        float offsetY = distr(gen);

        octaveOffsets[i] = glm::vec2(offsetX, offsetY);
    }


    m_VerticesVec3.resize(resolution * resolution);

    constexpr uint16_t NUM_THREADS = 8;
    std::cout << "THREADS: " << NUM_THREADS << std::endl;

    m_threadIndexVec.resize(NUM_THREADS);
    const unsigned int load = resolution / NUM_THREADS;

    for(int i = 0; i < NUM_THREADS; ++i)
    {
        m_threadIndexVec[i] = i*load*resolution;
    }

    Timer generateVerticesTimer;
    std::vector<std::thread> threads;
    for(int i = 0; i < NUM_THREADS; ++i)
    {
       threads.push_back(std::thread(&TerrainGenerator::GenerateVertices,this, i, NUM_THREADS, resolution, std::ref(terrainProperties), std::ref(octaveOffsets)));
    }

    for(int i = 0; i < NUM_THREADS; ++i)
    {
        threads[i].join();
    }
    generateVerticesTimer.End();
    std::cout << "Time spend generating vertices: " << generateVerticesTimer.duration.count() << "s " << std::endl;
    
    // Generate Faces
    // Rows (-1 for last)
    Timer triangleTimer;
    for(int r = 0; r < resolution - 1; ++r)
    {
        // Columns (-1 for last)
        for (int c = 0; c < resolution - 1; ++c)
        {
            // Upper triangle
            /*
                v0 -- v2
                |    /
                |  /
                v1
            */
            int f0_0 = (r * resolution) + c;
            int f0_1 = ((r + 1) * resolution) + c;
            int f0_2 = (r * resolution) + c + 1;
            
            glm::vec3 f0(f0_0, f0_1, f0_2);
            m_Triangles.push_back(f0);

            // Lower triangle
            /*
                      v2
                     / |
                   /   |
                v0 --- v1
            */
           int f1_0 = ((r + 1) * resolution) + c;
           int f1_1 = ((r + 1) * resolution) + c + 1;
           int f1_2 = (r * resolution) + c + 1;

           glm::vec3 f1(f1_0, f1_1, f1_2);
           m_Triangles.push_back(f1);
        }
    }
    triangleTimer.End();
    std::cout << "Time spent calculating triangles: " << triangleTimer.duration.count() << "s " << std::endl;

    Timer prepareVectorsTimer;
    //TODO: Add multithreading to this method, currently it's the other bottleneck in terrain generation performace.
    prepareVectors();
    prepareVectorsTimer.End();
    std::cout << "Time spent preparing vectors: " << prepareVectorsTimer.duration.count() << "s " << std::endl;

    Timer setVectorsTimer;
    setVectorsAndBuffers();
    setVectorsTimer.End();
    std::cout << "Timer spent setting vectors: " << setVectorsTimer.duration.count() << "s " << std::endl;

}

void TerrainGenerator::GenerateChunk(int resolution, TerrainProperties &terrainProperties)
{
    srand(terrainProperties.seed);
    std::mt19937 gen(terrainProperties.seed);
    std::uniform_int_distribution<> distr(-10000, 10000);

    std::vector<glm::vec2> octaveOffsets;
    octaveOffsets.reserve(terrainProperties.octaves);
    for(int i = 0; i < terrainProperties.octaves; ++i)
    {
        float offsetX = distr(gen);
        float offsetY = distr(gen);

        octaveOffsets[i] = glm::vec2(offsetX, offsetY);
    }

    m_VerticesVec3.reserve(resolution * resolution);
    float spacing = 1.f / (float(resolution) / 64.f);

    // Rows
    for(int r = 0; r < resolution; ++r)
    {
        // Columns
        for(int c = 0; c < resolution; ++c)
        {
            float x = c;
            float z = r;

            float xs = x / terrainProperties.scale;
            float zs = z / terrainProperties.scale;

            float amplitude = 1;
            float frequency = 1;
            float noiseHeight = 0;

            float perlinValue;
            for(int i = 0; i < terrainProperties.octaves; ++i)
            {
                float sampleX = xs * frequency + octaveOffsets[i].x;
                float sampleZ = zs * frequency + octaveOffsets[i].y;

                switch (terrainProperties.noiseType)
                {
                case 0:
                    perlinValue = glm::perlin(glm::vec2(sampleX, sampleZ));
                    break;
                case 1:
                    perlinValue = glm::simplex(glm::vec2(sampleX, sampleZ));
                    break;
                }
                
                noiseHeight += perlinValue * amplitude;

                amplitude = pow(terrainProperties.persistance, i);
                frequency = pow(terrainProperties.lacunarity, i);

            }

            noiseHeight *= terrainProperties.height;

            if(noiseHeight < terrainProperties.floor && terrainProperties.floor != 0.f)
                noiseHeight = terrainProperties.floor;

            float y = noiseHeight;
            glm::vec3 v(x * spacing, y * spacing, z * spacing);
            m_VerticesVec3.push_back(v);

            // Vertex Colors
            // m_VertexColorsVec3.push_back(glm::vec3(normY * 0.5, normY * 0.25, normY));
            // m_VertexColorsVec3.push_back(glm::vec3(0.3, 0.3, 0.3));
            m_VertexColorsVec3.push_back(terrainProperties.colorVec);

            // Create a vector in the mapping
            // To keep track of all the triangles
            std::vector<int> triangles;
            m_VertexToTrianglesMap.emplace(m_VerticesVec3.size() - 1, triangles);

        }
    }

    // Generate Faces
    // Rows (-1 for last)
    for(int r = 0; r < resolution - 1; ++r)
    {
        // Columns (-1 for last)
        for (int c = 0; c < resolution - 1; ++c)
        {
            // Upper triangle
            /*
                v0 -- v2
                |    /
                |  /
                v1
            */
            int f0_0 = (r * resolution) + c;
            int f0_1 = ((r + 1) * resolution) + c;
            int f0_2 = (r * resolution) + c + 1;
            
            glm::vec3 f0(f0_0, f0_1, f0_2);
            m_Triangles.push_back(f0);

            // Lower triangle
            /*
                      v2
                     / |
                   /   |
                v0 --- v1
            */
           int f1_0 = ((r + 1) * resolution) + c;
           int f1_1 = ((r + 1) * resolution) + c + 1;
           int f1_2 = (r * resolution) + c + 1;

           glm::vec3 f1(f1_0, f1_1, f1_2);
           m_Triangles.push_back(f1);
        }
    }

    prepareVectors();

    setVectorsAndBuffers();

}

void TerrainGenerator::prepareVectors()
{

    // Compute triangle normals
    Timer triangleNormalsTimer;
    m_TriangleNormals.reserve(m_Triangles.size());
    for(int i = 0; i < m_Triangles.size(); ++i)
    {
        int v0i = m_Triangles[i].x;
        int v1i = m_Triangles[i].y;
        int v2i = m_Triangles[i].z;
        
        // Link vertex to triangle
        m_VertexToTrianglesMap.at(v0i).push_back(i);
        m_VertexToTrianglesMap.at(v1i).push_back(i);
        m_VertexToTrianglesMap.at(v2i).push_back(i);

        glm::vec3 v0 = m_VerticesVec3[v0i];
        glm::vec3 v1 = m_VerticesVec3[v1i];
        glm::vec3 v2 = m_VerticesVec3[v2i];
        glm::vec3 tn = glm::triangleNormal(v0, v1, v2);
        m_TriangleNormals.emplace_back(tn);
    }
    triangleNormalsTimer.End();
    std::cout << "Time spent computing triangle normals: " << triangleNormalsTimer.duration.count() << "s " << std::endl;

    // Compute vertex normals
    Timer vertexNormalsTimer;
    m_VertexNormals.reserve(m_VerticesVec3.size());
    for(int i = 0; i < m_VerticesVec3.size(); ++i)
    {
        std::vector<int> triangle_indices = m_VertexToTrianglesMap[i];
        glm::vec3 v_sum = glm::vec3(0.0f);

        for(int j : triangle_indices)
        {
            v_sum = v_sum + m_TriangleNormals[j];
        }

        v_sum = glm::normalize(v_sum);
        m_VertexNormals.emplace_back(v_sum);
    }
    vertexNormalsTimer.End();
    std::cout << "Time spent computing vertex normals: " << vertexNormalsTimer.duration.count() << "s " << std::endl;
}

void TerrainGenerator::setVectorsAndBuffers()
{
    int stride = 9;
    m_Vertices.resize(m_VerticesVec3.size() * 9);
    
    for(int i = 0; i < m_VerticesVec3.size(); ++i)
    {
        m_Vertices[(i * stride)] = m_VerticesVec3[i].x;
        m_Vertices[(i * stride)+1] = m_VerticesVec3[i].y;
        m_Vertices[(i * stride)+2] = m_VerticesVec3[i].z;

        m_Vertices[(i * stride)+3] = m_VertexColorsVec3[i].x;
        m_Vertices[(i * stride)+4] = m_VertexColorsVec3[i].y;
        m_Vertices[(i * stride)+5] = m_VertexColorsVec3[i].z;

        m_Vertices[(i * stride)+6] = m_VertexNormals[i].x;
        m_Vertices[(i * stride)+7] = m_VertexNormals[i].y;
        m_Vertices[(i * stride)+8] = m_VertexNormals[i].z;
    }

    for (glm::vec3 triangle : m_Triangles)
    {
        m_Indices.push_back((uint32_t)(triangle.x));
        m_Indices.push_back((uint32_t)(triangle.y));
        m_Indices.push_back((uint32_t)(triangle.z));
    }


}