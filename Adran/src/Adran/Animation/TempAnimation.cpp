#include "AdranPreCompile.h"
#include "Animation.h"

namespace Adran
{
    Animation::Animation(float duration, uint32_t samples)
        : m_duration(duration), m_samples(samples)
    {
        AR_ASSERT(duration != 0.0f, "Animation 的Duration不可以是0");
        AR_ASSERT(samples > 0, "Animation 的Samples不可以小于等于0");
        m_gap = m_duration / static_cast<float>(m_samples);
        m_positionLineMap[0] = glm::vec2(0.0f);
        m_rotationLineMap[0] = 0.0f;
        m_scaleLineMap[0] = glm::vec2(1.0f);
    }

    void Animation::AddPositionKeyFrame(uint32_t key, const glm::vec2& position)
    {
        m_positionLineMap[key] = position;
    }

    void Animation::AddRotationKeyFrame(uint32_t key, float rotation)
    {
        m_rotationLineMap[key] = rotation;
    }

    void Animation::AddScaleKeyFrame(uint32_t key, const glm::vec2& scale)
    {
        m_scaleLineMap[key] = scale;
    }

    void Animation::OnUpdate(TimeStep ts)
    {
        m_timeContainer += ts;

        // 处理循环动画
        while (m_timeContainer >= m_duration)
        {
            m_timeContainer -= m_duration;
        }

        UpdateKey(m_timeContainer);

        float progress = m_timeContainer / m_duration;

        InterpolatePosition(progress);
        InterpolateRotation(progress);
        InterpolateScale(progress);
    }

    void Animation::InterpolatePosition(float progress)
    {
        uint32_t prevKey = m_lastPositionKey;
        uint32_t nextKey = m_nextPositionKey;

        glm::vec2 prevPosition = m_positionLineMap[prevKey];
        glm::vec2 nextPosition = m_positionLineMap[nextKey];

        m_position = glm::mix(prevPosition, nextPosition, progress);
    }

    void Animation::InterpolateRotation(float progress)
    {
        uint32_t prevKey = m_lastRotationKey;
        uint32_t nextKey = m_nextRotationKey;

        float prevRotation = m_rotationLineMap[prevKey];
        float nextRotation = m_rotationLineMap[nextKey];

        m_rotation = glm::mix(prevRotation, nextRotation, progress);
    }

    void Animation::InterpolateScale(float progress)
    {
        uint32_t prevKey = m_lastScaleKey;
        uint32_t nextKey = m_nextScaleKey;

        glm::vec2 prevScale = m_scaleLineMap[prevKey];
        glm::vec2 nextScale = m_scaleLineMap[nextKey];

        m_scale = glm::mix(prevScale, nextScale, progress);
    }

    void Animation::UpdateKey(float timeContainer)
    {
        // 更新当前帧和下一帧
        m_lastPositionKey = m_nextPositionKey;
        m_nextPositionKey = FindNextKey(m_positionLineMap, m_lastPositionKey);

        m_lastRotationKey = m_nextRotationKey;
        m_nextRotationKey = FindNextKey(m_rotationLineMap, m_lastRotationKey);

        m_lastScaleKey = m_nextScaleKey;
        m_nextScaleKey = FindNextKey(m_scaleLineMap, m_lastScaleKey);
    }

    uint32_t Animation::FindNextKey(const std::map<uint32_t, glm::vec2>& keyMap, uint32_t currentKey)
    {
        uint32_t result = m_samples;
        for (const auto& it : keyMap)
        {
            if (it.first > currentKey && it.first < result)
            {
                result = it.first;
            }
        }
        return result == m_samples ? 0 : result;
    }

    uint32_t Animation::FindNextKey(const std::map<uint32_t, float>& keyMap, uint32_t currentKey)
    {
        uint32_t result = m_samples;
        for (const auto& it : keyMap)
        {
            if (it.first > currentKey && it.first < result)
            {
                result = it.first;
            }
        }
        return result == m_samples ? 0 : result;
    }
}
