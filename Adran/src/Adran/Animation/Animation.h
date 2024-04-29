#pragma once

#include "Adran/Core/Core.h"
#include "Adran/Core/TimeStep.h"

#include <glm/glm.hpp>
#include <vector>
#include <map>

namespace Adran
{
    class Animation
    {
    public:
        Animation(float duration, uint32_t samples);

        void AddPositionKeyFrame(uint32_t key, const glm::vec2& position);
        void AddRotationKeyFrame(uint32_t key, float rotation);
        void AddScaleKeyFrame(uint32_t key, const glm::vec2& scale);
        void AddTextureKeyFrame(uint32_t key, const std::string& path);

        const glm::vec2& GetPosition() const { return m_position; }
        float GetRotation() const { return m_rotation; }
        const glm::vec2& GetScale() const { return m_scale; }
        const std::string& GetTexturePath() const { return m_texturePath; }

        void OnUpdate(TimeStep ts);

    private:
        void UpdateKeyFrames();
        void InterpolatePosition(float progress);
        void InterpolateRotation(float progress);
        void InterpolateScale(float progress);
        float GetTimeRatio(uint32_t lastKey, uint32_t nextKey, float TheTime);

    private:
        std::map<uint32_t, glm::vec2> m_positionLineMap;
        std::map<uint32_t, float> m_rotationLineMap;
        std::map<uint32_t, glm::vec2> m_scaleLineMap;
        std::map<uint32_t, std::string> m_texturePathLineMap;

        uint32_t m_lastPositionKey = 0;
        uint32_t m_nextPositionKey = 0;
        uint32_t m_lastRotationKey = 0;
        uint32_t m_nextRotationKey = 0;
        uint32_t m_lastScaleKey = 0;
        uint32_t m_nextScaleKey = 0;

        uint32_t m_lastTextureKey = 0;

        glm::vec2 m_position = { 0.0f, 0.0f };
        float m_rotation = 0.0f;
        glm::vec2 m_scale = { 0.0f, 0.0f };

        std::string m_texturePath = "";

        float m_gap;
        float m_timeContainer = 0.0f;
        float m_duration;
        uint32_t m_samples;
    };
}
