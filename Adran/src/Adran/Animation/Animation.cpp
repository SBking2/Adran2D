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
        m_texturePathLineMap[0] = std::string();

        m_gap = m_duration / m_samples;
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

    void Animation::AddTextureKeyFrame(uint32_t key, const std::string& path)
    {
        m_texturePathLineMap[key] = path;
    }

    void Animation::OnUpdate(TimeStep ts)
    {
        m_timeContainer += ts;

        // 处理循环动画
        while (m_timeContainer >= m_duration)
        {
            m_timeContainer -= m_duration;
        }

        UpdateKeyFrames();

        float PositionProgress = GetTimeRatio(m_lastPositionKey, m_nextPositionKey, m_timeContainer);
        float RotationProgress = GetTimeRatio(m_lastRotationKey, m_nextRotationKey, m_timeContainer);;
        float ScaleProgress = GetTimeRatio(m_lastScaleKey, m_nextScaleKey, m_timeContainer);;

        InterpolatePosition(PositionProgress);
        InterpolateRotation(RotationProgress);
        InterpolateScale(ScaleProgress);

        m_texturePath = m_texturePathLineMap[m_lastTextureKey];
    }

    void Animation::UpdateKeyFrames()
    {
        m_lastPositionKey = 0;
        m_nextPositionKey = 0;
        m_lastRotationKey = 0;
        m_nextRotationKey = 0;
        m_lastScaleKey = 0;
        m_nextScaleKey = 0;

        m_lastTextureKey = 0;

        // 找到当前时间对应的关键帧
        for (auto it = m_positionLineMap.begin(); it != m_positionLineMap.end(); ++it)
        {
            if (it->first * m_gap <= m_timeContainer)
            {
                m_lastPositionKey = it->first;
            }
            else
            {
                m_nextPositionKey = it->first;
                break;
            }
        }


        for (auto it = m_rotationLineMap.begin(); it != m_rotationLineMap.end(); ++it)
        {
            if (it->first * m_gap <= m_timeContainer)
            {
                m_lastRotationKey = it->first;
            }
            else
            {
                m_nextRotationKey = it->first;
                break;
            }
        }

        for (auto it = m_scaleLineMap.begin(); it != m_scaleLineMap.end(); ++it)
        {
            if (it->first * m_gap <= m_timeContainer)
            {
                m_lastScaleKey = it->first;
            }
            else
            {
                m_nextScaleKey = it->first;
                break;
            }
        }

        //更新Sprite Key
        for (auto it = m_texturePathLineMap.begin(); it != m_texturePathLineMap.end(); ++it)
        {
            if (it->first * m_gap <= m_timeContainer)
            {
                m_lastTextureKey = it->first;
            }
        }
    }

    void Animation::InterpolatePosition(float progress)
    {
        glm::vec2 prevPosition = m_positionLineMap[m_lastPositionKey];
        glm::vec2 nextPosition = m_positionLineMap[m_nextPositionKey];

        m_position = glm::mix(prevPosition, nextPosition, progress);
    }

    void Animation::InterpolateRotation(float progress)
    {
        float prevRotation = m_rotationLineMap[m_lastRotationKey];
        float nextRotation = m_rotationLineMap[m_nextRotationKey];

        m_rotation = glm::mix(prevRotation, nextRotation, progress);
    }

    void Animation::InterpolateScale(float progress)
    {
        glm::vec2 prevScale = m_scaleLineMap[m_lastScaleKey];
        glm::vec2 nextScale = m_scaleLineMap[m_nextScaleKey];

        m_scale = glm::mix(prevScale, nextScale, progress);
    }

    float Animation::GetTimeRatio(uint32_t lastKey, uint32_t nextKey, float TheTime)
    {
        float allTime = nextKey == 0 ? (m_samples - lastKey) * m_gap : (nextKey - lastKey) * m_gap;
        float overTime = TheTime - lastKey * m_gap;

        if (allTime == 0.0f)
        {
            return 0.0f;
        }

        return overTime / allTime;
    }
}
