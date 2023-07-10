#version 410 core
out vec4 FragColor;

uniform vec3 CameraPosition;
uniform sampler2D texture1;
uniform sampler2D Chunk;
uniform vec3 LightDirection;
uniform float AmbientLight;

uniform int Chunkheight;
uniform int Chunkdepth;
uniform int Chunkwidth;
uniform float EnergyLoss;

in vec3 ourColor;
in vec3 Normal;
in vec4 Pos;
in vec3 FragPos;
in vec2 TexCord;




struct RayHit
{
	float Distance;
	vec3 Hitpoint;
	vec3 Normal;
	ivec3 BlockHit;
};


int GetTextureIndex(ivec3 position)
{
	if(position.x > Chunkwidth || position.y > Chunkheight || position.z > Chunkdepth || position.x < 0 || position.y < 0 || position.z < 0){
		return 0;
	}
	vec2 index = vec2(position.x, Chunkheight * position.z + position.y);
	float value = texture(Chunk, index/vec2(Chunkwidth, Chunkdepth * Chunkheight)).r;


	return int(value * 255);
}
RayHit RayChunkCast(vec3 RayDir, vec3 RayOrigin, float dist) {

	

	// px + dx * t = 2;
	// px/dx + t = 2/dx
	// px/dx = 2/dx - t

	// px/dx - 2/dx = -t;
	






	float t = -1; 
	vec3 Normal = vec3(0, 0, 0);
	ivec3 HitBlock = ivec3(0,0,0);
	//horizonal


		//we loop 3 times switching between x, y then z
	float Starter = RayOrigin.x;
	modf(Starter, Starter);
	int WallDir = -1;

	float Direction = RayDir.x;

	float Origin = RayOrigin.x;
	if (Direction > 0) {
		Starter += 1;
		WallDir = 1;
	}

		vec3 TempNormal = vec3(0, 0, 0);
		TempNormal.x = -WallDir;
	

		for (int x = 0; x < int(dist) + 1; x++) {
			//calculate intersection point

			float Distance = ((Starter + (x * WallDir)) / Direction) - (Origin / Direction);

			vec3 BlockHit = RayOrigin + RayDir * Distance;

			BlockHit -= TempNormal / 100.0f;

			if (GetTextureIndex(ivec3(BlockHit)) != 0) {

				if ((Distance < t || t < 0) && Distance < dist && Distance > 0) {
					t = Distance;
					Normal = TempNormal;
					HitBlock = ivec3(BlockHit);
					
				}
				break;
			}
		}
	//vertical
	Starter = RayOrigin.y;
	modf(Starter, Starter);
	WallDir = -1;

	Direction = RayDir.y;

	Origin = RayOrigin.y;
	if (Direction > 0) {
		Starter += 1;
		WallDir = 1;
	}

	TempNormal = vec3(0, 0, 0);
	TempNormal.y = -WallDir;
	

	for (int y = 0; y < int(dist) + 1; y++) {
		//calculate intersection point

		float Distance = ((Starter + (y * WallDir)) / Direction) - (Origin / Direction);

		vec3 BlockHit = RayOrigin + RayDir * Distance;

		BlockHit -= TempNormal / 100.0f;

		if (GetTextureIndex(ivec3(BlockHit)) != 0) {

			if ((Distance < t || t < 0) && Distance < dist && Distance > 0) {
				t = Distance;
				Normal = TempNormal;
				HitBlock = ivec3(BlockHit);
				
				}
				break;
			}
		}
	//depth
	Starter = RayOrigin.z;
	modf(Starter, Starter);
	WallDir = -1;

	Direction = RayDir.z;

	Origin = RayOrigin.z;
	if (Direction > 0) {
		Starter += 1;
		WallDir = 1;
	}

	TempNormal = vec3(0, 0, 0);
	TempNormal.z = -WallDir;
	

	for (int z = 0; z < int(dist) + 1; z++) {
		//calculate intersection point

		float Distance = ((Starter + (z * WallDir)) / Direction) - (Origin / Direction);

		vec3 BlockHit = RayOrigin + RayDir * Distance;

		BlockHit -= TempNormal / 100.0f;

		if (GetTextureIndex(ivec3(BlockHit)) != 0) {

			if ((Distance < t || t < 0) && Distance < dist && Distance > 0) {
				t = Distance;
				Normal = TempNormal;
				HitBlock = ivec3(BlockHit);
				
				}
				break;
			}
		}
	vec3 Hit = RayOrigin + RayDir * t;





	/*
	struct RayHit {
		float Distance;
		glm::vec3 Normal;
		glm::vec3 HitPoint;
		glm::ivec3 BlockHit;
	};
	
	*/
	RayHit ray;

	ray.Distance = t;
	ray.Hitpoint = Hit;
	ray.Normal = Normal;
	ray.BlockHit = HitBlock;

	return ray;
}




void main()
{

	
	vec4 Color = texture(texture1, TexCord) ;

	RayHit ray;

	vec3 Position = FragPos + Normal * 0.0001;
	

	ray = RayChunkCast(-normalize(Position - LightDirection), Position, 64);
	if(ray.Distance != -1){

		Color *= vec4(EnergyLoss, EnergyLoss, EnergyLoss, 1.0);
	}
	else
	{
		Color *= vec4(vec3(1,1,1)/sqrt(distance(Position, LightDirection)), 1.0);
	}

	
	

	
	if(Color.a < 0.5){
		discard;
	}

	FragColor = Color;
} 