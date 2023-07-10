
//for creating offset while instancing
uint wang_hash(uint seed);
vec3 RandomSpherePosition(vec3 Center, float radius); 
//unique id for the chunk that is being rendered
uniform vec3 ChunkSeed;
uniform float Chunksize;
uniform vec3 BaseChunkPos;

uint SeedX;
uint SeedY;
uint SeedZ;

void main()
{


    
    PositionOffset = vec3(0,0,0);

}

uint wang_hash(uint seed)
{
    seed = (seed ^ 61) ^ (seed >> 16);
    seed *= 9;
    seed = seed ^ (seed >> 4);
    seed *= 0x27d4eb2d;
    seed = seed ^ (seed >> 15);
    return seed;
}