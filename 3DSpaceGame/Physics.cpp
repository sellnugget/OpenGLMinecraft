#include "Physics.h"

#include <map>
#include <math.h>
RayHit RayChunkCast(Chunk* chunk, glm::vec3 RayDir, glm::vec3 RayOrigin, float distance) {

	

	// px + dx * t = 2;
	// px/dx + t = 2/dx
	// px/dx = 2/dx - t

	// px/dx - 2/dx = -t;
	






	float t = RAY_MISSED; 
	glm::vec3 Normal = glm::vec3(0, 0, 0);
	glm::ivec3 HitBlock = glm::vec3(0,0,0);
	for (int Axis = 0; Axis < sizeof(glm::vec3)/sizeof(float); Axis++) {
		//horizonal


		//we loop 3 times switching between x, y then z
		float Starter = ((float*)&RayOrigin)[Axis];
		std::modf(Starter, &Starter);
		int WallDir = -1;

		float Direction = ((float*)&RayDir)[Axis];

		float Origin = ((float*)&RayOrigin)[Axis];
		if (Direction > 0) {
			Starter += 1;
			WallDir = 1;
		}

		glm::vec3 TempNormal = glm::vec3(0, 0, 0);
		((float*)&TempNormal)[Axis] = -WallDir;
	

		for (int x = 0; x < (int)distance + 1; x++) {
			//calculate intersection point

			float Distance = ((Starter + (x * WallDir)) / Direction) - (Origin / Direction);

			glm::vec3 BlockHit = RayOrigin + RayDir * Distance;

			BlockHit -= TempNormal / 100.0f;

			if (chunk->GetBlock(BlockHit) != 0) {

				if ((Distance < t || t < 0) && Distance < distance && Distance > 0) {
					t = Distance;
					Normal = TempNormal;
					HitBlock = BlockHit;
					
				}
				break;
			}
		}
	}

	glm::vec3 Hit = RayOrigin + RayDir * t;





	/*
	struct RayHit {
		float Distance;
		glm::vec3 Normal;
		glm::vec3 HitPoint;
		glm::ivec3 BlockHit;
	};
	
	*/

	return { t, Normal, Hit, HitBlock};

}