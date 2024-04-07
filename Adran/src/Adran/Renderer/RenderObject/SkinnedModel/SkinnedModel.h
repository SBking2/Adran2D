#pragma once

#include "SkinnedMesh.h"

#define NUM_BONES_PER_VERTEX 4

namespace Adran
{


	class SkinnedModel
	{
		struct BoneData {
			uint32_t boneIndex[NUM_BONES_PER_VERTEX];
			float weights[NUM_BONES_PER_VERTEX];
			void Add(uint32_t boneID, float weight) {
				for (size_t i = 0; i < NUM_BONES_PER_VERTEX; i++) {
					if (weights[i] == 0.0f) {
						boneIndex[i] = boneID;
						weights[i] = weight;
						return;
					}
				}
				//insert error program
				AR_CORE_ERROR("¹Ç÷ÀË÷ÒýOverFlow");
			}
		};
	};