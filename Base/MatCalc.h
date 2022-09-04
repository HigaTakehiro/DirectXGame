#pragma once
#include <DirectXMath.h>

using namespace DirectX;

class MatCalc final
{
private:
	/// <summary>
    /// �R���X�g���N�^
    /// </summary>
	MatCalc() = default;

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~MatCalc() = default;

	/// <summary>
	/// �R�s�[�R���X�g���N�^���֎~
	/// </summary>
	MatCalc(const MatCalc & obj) = delete;

	/// <summary>
	/// ������Z�q���֎~
	/// </summary>
	MatCalc& operator=(const MatCalc & obj) = delete;

public: //�ÓI�����o�֐�
	/// <summary>
	/// �C���X�^���X���擾
	/// </summary>
	/// <returns>�C���X�^���X</returns>
	static MatCalc* GetIns();

public: //�����o�֐�

	/// <summary>
	/// ���W�ɍs����|���čŌ��w���Z����
	/// </summary>
	/// <param name="vec">���W</param>
	/// <param name="mat">�s��</param>
	/// <returns>�v�Z����</returns>
	XMVECTOR Wdivided(XMVECTOR vec, XMMATRIX mat);

	/// <summary>
	/// �x�N�g���ƍs��̊|���Z
	/// </summary>
	/// <param name="vec">�x�N�g��</param>
	/// <param name="mat">�s��</param>
	/// <returns>�v�Z����</returns>
	XMVECTOR VecDivided(XMVECTOR vec, XMMATRIX mat);

};

