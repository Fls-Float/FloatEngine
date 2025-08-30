/**
 * @file Sprite.h
 * @brief ��Ϸ������
 * @defgroup Sprite ����ģ��
 */
#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include "raylib.hpp"
#include "F_Resource.h"
class Sprite {
private:
    std::vector<Texture> frames; ///< �洢֡������
    size_t currentFrame = 0; ///< ��ǰ֡������
    bool isOwner = true; ///< �Ƿ�ӵ��֡��Դ������Ȩ

public:
    /**
     * @brief Ĭ�Ϲ��캯��
     */
    Sprite();

    /**
     * @brief ʹ�õ���������Sprite
     * @param texture ����
     * @param takeOwnership �Ƿ�ӵ��������Դ������Ȩ
     */
    explicit Sprite(Texture2D texture, bool takeOwnership = true);

    /**
     * @brief ʹ���ļ�·���б���Sprite
     * @param filePaths �ļ�·���б�
     */
    explicit Sprite(const std::vector<const char*>& filePaths);

    /**
     * @brief ��������
     */
    ~Sprite();

    /**
     * @brief �������캯��
     */
    Sprite(const Sprite&other) ;

    /**
     * @brief ������ֵ�����
     */
    Sprite& operator=(const Sprite& other);

    /**
     * @brief �ƶ����캯��
     * @param other Ҫ�ƶ��Ķ���
     */
    Sprite(Sprite&& other) noexcept;

    /**
     * @brief �ƶ���ֵ�����
     * @param other Ҫ�ƶ��Ķ���
     * @return �ƶ���Ķ�������
     */
    Sprite& operator=(Sprite&& other) noexcept;

    /**
     * @brief ���֡
     * @param texture Ҫ��ӵ�����
     */
    void AddFrame(const Texture2D&& texture);

    /**
     * @brief ���ļ�����֡
     * @param filePath �ļ�·��
     * @return ���سɹ�����true�����򷵻�false
     */
    bool LoadFrame(const char* filePath);
    /**
    * @brief ��F_Resource����֡
    * @param resource ��Դ��
    * @param name ��Դ����
    * @return ���سɹ�����true�����򷵻�false
    */
    bool LoadFrameFromResource(const F_Resource* resource, const char* name);
    /**
     * @brief ���õ�ǰ֡
     * @param index ֡����
     */
    void SetFrame(size_t index);
    /**
	* @brief ����֡������+1��������Χ������Ϊ0
    */
    void UpdateFrameIndex();
    /**
    * @brief �޸�֡����
    * @param index ֡����
    */
    void SetFrameTexture(size_t index,const Texture& tex);
    /**
     * @brief ��ȡ��ǰ֡
     * @return ��ǰ֡������
     */
    Texture2D GetCurrentFrame() const;

    /**
     * @brief ��ȡ��ǰ֡
     * @return ��ǰ֡������
     */
    Texture2D GetFrameIndex(int index) const;
    /**
    * @brief ��ȡ����֡
    * @return ����֡������
    */
    Texture2D* GetAllFrame() const;
    /**
     * @brief ��ȡ֡������
     * @return ֡������
     */
    size_t FrameCount() const;
    
};
