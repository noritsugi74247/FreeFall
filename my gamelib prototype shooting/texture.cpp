// UNIT.04
#include "texture.h"
#include "misc.h"
#include "shader.h"
#include "util.h"

#include "texture.h"
#include "mylib.h"
#include <WICTextureLoader.h>
#include <wrl.h>
#include <map>
#include <sstream>

HRESULT load_texture_from_file(ID3D11Device* device, const wchar_t* file_name, ID3D11ShaderResourceView** shader_resource_view, D3D11_TEXTURE2D_DESC* texture2d_desc)
{
	HRESULT hr = S_OK;
	Microsoft::WRL::ComPtr<ID3D11Resource> resource;

	static std::map<std::wstring, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> cache;
	auto it = cache.find(file_name);
	if (it != cache.end())
	{
		//it->second.Attach(*shader_resource_view);
		*shader_resource_view = it->second.Get();
		(*shader_resource_view)->AddRef();
		(*shader_resource_view)->GetResource(resource.GetAddressOf());
	}

	else
	{ 
	hr = DirectX::CreateWICTextureFromFile(device, file_name, resource.GetAddressOf(),
		shader_resource_view);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	cache.insert(std::make_pair(file_name, *shader_resource_view));
	}


	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2d;
	hr = resource.Get()->QueryInterface<ID3D11Texture2D>(texture2d.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	texture2d->GetDesc(texture2d_desc);

	return hr;
}




ResourceManager::ShaderResourceViewData	ResourceManager::srvData[ResourceManager::RESOURCE_MAX];
ResourceManager::VertexShadersData		ResourceManager::vsData[ResourceManager::RESOURCE_MAX];
ResourceManager::PixelShadersData		ResourceManager::psData[ResourceManager::RESOURCE_MAX];


//	CSO�t�@�C���ǂݍ��݊֐�
bool	LoadCSOFile(BYTE** data, long* size, const char* filename)
{
	FILE* fp;
	//	�t�@�C���I�[�v��
	if (fopen_s(&fp, filename, "rb"))	return false;

	//	�t�@�C���T�C�Y�擾
	fseek(fp, 0, SEEK_END);		//	�t�@�C���|�C���^���I�[�Ɉړ�
	long sz = ftell(fp);		//	�I�[�ʒu�̕ۑ� = �t�@�C���T�C�Y
	fseek(fp, 0, SEEK_SET);		//	�t�@�C���|�C���^���ŏ��ɖ߂�

								//	�t�@�C���ǂݍ��ݗp�̈�̊m��
	*data = new BYTE[sz];
	//	�t�@�C���ǂݍ���
	size_t s = fread_s(*data, sizeof(BYTE) * sz,
		sizeof(BYTE), sz, fp);

	//	�ǂݍ��ݏI��
	fclose(fp);

	//	�G���[�`�F�b�N
	if (s != sizeof(BYTE) * sz)	return false;

	*size = sz;			//	�t�@�C���T�C�Y�ۑ�
	return	true;
}



bool ResourceManager::LoadShaderResourceView(ID3D11Device* device, const wchar_t* filename,
	ID3D11ShaderResourceView** srv, D3D11_TEXTURE2D_DESC** texDesc)
{
	//	�G���[�`�F�b�N
	if (!device)		return false;
	if (!filename)		return false;
	if (*srv)			return false;

	HRESULT hr;
	//	ID3D11Resource* resource = nullptr;;
	ShaderResourceViewData* find = nullptr;
	int no = -1;

	//	�Ώۂ̃t�@�C�������ɑ��݂��Ă��邩������
	for (int n = 0; n < RESOURCE_MAX; n++)
	{
		ShaderResourceViewData* p = &srvData[n];

		//	�f�[�^�̗L���m�F
		if (p->count <= 0)
		{
			if (no == -1)		no = n;
			continue;
		}

		//	�������r
		if (wcscmp(p->filename, filename) != 0)	continue;

		//	��v��������ǂݍ���ł� => ����!
		find = p;

		break;
	}

	//	�t�@�C����������Ȃ�������V�K�ǂݍ���
	if (!find)
	{
		if (no == -1)	return	false;		//	�󂫂�����

		find = &srvData[no];
		//	�e�N�X�`���ǂݍ���
		ID3D11Resource* resource = nullptr;
		hr = DirectX::CreateWICTextureFromFile(device, filename, &resource,
			&find->shader_resource_view);
		//		���f�[�^���ǂݍ��߂Ȃ������� return false;
		if (FAILED(hr))	return false;

		//	texture2d_desc�̎擾
		ID3D11Texture2D* texture2d;
		hr = resource->QueryInterface(&texture2d);
		//		���f�[�^���ǂݍ��߂Ȃ������� return false;
		if (FAILED(hr))
		{
			resource->Release();
			find->shader_resource_view->Release();
			return false;
		}
		//	texture2d_desc�ۑ�
		texture2d->GetDesc(&find->texDesc);

		//	�|�C���^�ϐ� ���
		texture2d->Release();
		resource->Release();

		//	�t�@�C�����̃R�s�[
		wcscpy_s(find->filename, 256, filename);
	}

	//////////////////////////
	//	���ʕ�
	//////////////////////////

	//	�f�[�^��Ԃ�
	*srv = find->shader_resource_view;	//	ShaderResourceView�ۑ�
	*texDesc = &find->texDesc;				//	texture2d_desc�ۑ�

	//	�ێ�������
	find->count++;


	return	true;
}

void ResourceManager::ReleaseShaderResourceView(ID3D11ShaderResourceView* srv)
{
	if (!srv)			return;		//	�����Ώ�Unknown�@�I�����܂�

	//	Target�̌����J�n
	for (int n = 0; n < RESOURCE_MAX; n++)
	{
		ShaderResourceViewData* p = &srvData[n];

		//	���g�p�f�[�^�Ȃ�A���̃f�[�^�ֈڍs���܂�
		if (p->count == 0)	continue;
		//	�����Ώۂƕs��v�Ȃ�A���̃f�[�^�ֈڍs���܂�
		if (p->shader_resource_view != srv)	continue;

		//	Target�𔭌� Release���܂�
		p->Release();
		break;
	}
}


bool ResourceManager::LoadVertexShaders(ID3D11Device* device, const char* filename,
	D3D11_INPUT_ELEMENT_DESC* elementDescs, int numElement,
	ID3D11VertexShader** vs, ID3D11InputLayout** il)
{
	//	�G���[�`�F�b�N
	if (!device)		return false;
	if (!filename)		return false;
	if (*vs)			return false;
	if (*il)			return false;

	HRESULT hr;
	VertexShadersData* find = nullptr;
	int no = -1;

	//	�Ώۂ̃t�@�C�������ɑ��݂��Ă��邩������
	for (int n = 0; n < RESOURCE_MAX; n++)
	{
		VertexShadersData* p = &vsData[n];

		//	�f�[�^�̗L���m�F
		if (p->count == 0)
		{
			if (no == -1)	no = n;
			continue;
		}
		//	�������r
		if (strcmp(p->filename, filename) != 0)	continue;

		//	��v��������ǂݍ���ł� => ����!
		find = p;
		//	���łɍ����̂�����΂�����
		//		�� VertexShaders�̏ꍇ�A���ɖ���
		break;
	}

	//	�t�@�C����������Ȃ�������V�K�ǂݍ���
	if (!find)
	{
		if (no == -1)	return	false;		//	�󂫂�����

		find = &vsData[no];
		//	�V�K�ǂݍ���
		BYTE* shader_data;
		long				size;
		LoadCSOFile(&shader_data, &size, filename);
		hr = device->CreateVertexShader(shader_data, size, nullptr, &find->vertex_shader);
		//		���f�[�^���ǂݍ��߂Ȃ������� return false;
		if (FAILED(hr))	return	false;

		hr = device->CreateInputLayout(elementDescs, numElement,
			shader_data, size, &find->input_layout);
		delete[] shader_data;
		//		���f�[�^���ǂݍ��߂Ȃ������� return false;
		if (FAILED(hr))
		{
			find->vertex_shader->Release();
			return false;
		}

		//	�t�@�C�����̃R�s�[
		strcpy_s(find->filename, 256, filename);
	}

	//////////////////////////
	//	���ʕ�
	//////////////////////////

	//	���ʂ��č����̂�����΂����ō��
	//		��VertexShaders�œ��ɂ�鎖�͖���

	//	�f�[�^��Ԃ�
	*vs = find->vertex_shader;
	*il = find->input_layout;

	//	�ێ�������
	find->count++;

	return	true;
}

void ResourceManager::ReleaseVertexShaders(ID3D11VertexShader* vs, ID3D11InputLayout* il)
{
	if (!vs)			return;		//	�����Ώ�Unknown�@�I�����܂�
	if (!il)			return;		//	�����Ώ�Unknown�@�I�����܂�

	//	Target�̌����J�n
	for (int n = 0; n < RESOURCE_MAX; n++)
	{
		VertexShadersData* p = &vsData[n];

		//	���g�p�f�[�^�Ȃ�A���̃f�[�^�ֈڍs���܂�
		if (p->count == 0)	continue;
		//	�����Ώۂƕs��v�Ȃ�A���̃f�[�^�ֈڍs���܂�
		if (p->vertex_shader != vs)	continue;
		if (p->input_layout != il)	continue;

		//	Target�𔭌� Release���܂�
		p->Release();
		break;
	}
}



bool ResourceManager::LoadPixelShaders(ID3D11Device* device, const char* filename, ID3D11PixelShader** ps)
{
	//	�G���[�`�F�b�N
	if (!device)		return false;
	if (!filename)		return false;
	if (*ps)			return false;

	HRESULT hr;
	PixelShadersData* find = nullptr;
	int no = -1;

	//	�Ώۂ̃t�@�C�������ɑ��݂��Ă��邩������
	for (int n = 0; n < RESOURCE_MAX; n++)
	{
		PixelShadersData* p = &psData[n];

		//	�f�[�^�̗L���m�F
		if (p->count == 0)
		{
			if (no == -1)	no = n;
			continue;
		}
		//	�������r
		if (strcmp(p->filename, filename) != 0)	continue;

		//	��v��������ǂݍ���ł� => ����!
		find = p;
		//	���łɍ����̂�����΂�����
		//		�� VertexShaders�̏ꍇ�A���ɖ���
		break;
	}

	//	�t�@�C����������Ȃ�������V�K�ǂݍ���
	if (!find)
	{
		if (no == -1)	return	false;		//	�󂫂�����

		find = &psData[no];
		//	�V�K�ǂݍ���
		BYTE* shader_data;
		long				size;
		LoadCSOFile(&shader_data, &size, filename);
		hr = device->CreatePixelShader(shader_data, size, nullptr, &find->pixel_shader);
		//		���f�[�^���ǂݍ��߂Ȃ������� return false;
		if (FAILED(hr))	return	false;

		//	�t�@�C�����̃R�s�[
		strcpy_s(find->filename, 256, filename);

		delete[]	shader_data;
	}

	//////////////////////////
	//	���ʕ�
	//////////////////////////

	//	���ʂ��č����̂�����΂����ō��
	//		��VertexShaders�œ��ɂ�鎖�͖���

	//	�f�[�^��Ԃ�
	*ps = find->pixel_shader;

	//	�ێ�������
	find->count++;

	return	true;
}

void ResourceManager::ReleasePixelShaders(ID3D11PixelShader* ps)
{
	if (!ps)			return;		//	�����Ώ�Unknown�@�I�����܂�

									//	Target�̌����J�n
	for (int n = 0; n < RESOURCE_MAX; n++)
	{
		PixelShadersData* p = &psData[n];

		//	���g�p�f�[�^�Ȃ�A���̃f�[�^�ֈڍs���܂�
		if (p->count == 0)	continue;
		//	�����Ώۂƕs��v�Ȃ�A���̃f�[�^�ֈڍs���܂�
		if (p->pixel_shader != ps)	continue;

		//	Target�𔭌� Release���܂�
		p->Release();
		break;
	}
}





namespace mylib
{

	//==========================================================================
	//
	//      �e�N�X�`���Ǘ��N���X
	//
	//==========================================================================

	//--------------------------------------------------------------
	//  �R���X�g���N�^
	//--------------------------------------------------------------
	TextureManager::TextureManager() : maxNum(0), texture(nullptr), current(0)
	{
		init(system::TEXTURE_NUM);
	}

	//--------------------------------------------------------------
	//  �f�X�g���N�^
	//--------------------------------------------------------------
	TextureManager::~TextureManager()
	{
		uninit();
	}

	//--------------------------------------------------------------
	//  ������
	//--------------------------------------------------------------
	void TextureManager::init(size_t max)
	{
		uninit();	// �O�̂���

		texture = new Texture[max];
		maxNum = max;
	}

	//--------------------------------------------------------------
	//  �I������
	//--------------------------------------------------------------
	void TextureManager::uninit()
	{
		if (texture)
		{
			releaseAll();
			safe_delete_array(texture);
			maxNum = 0;
		}
	}

	//--------------------------------------------------------------
	//  �e�N�X�`�����[�h�i�ԍ��w��j
	//--------------------------------------------------------------
	void TextureManager::load(ID3D11Device* device, int i, const wchar_t* fileName, size_t maxInstance)
	{
		if ((size_t)i < maxNum) texture[i].load(device, fileName, maxInstance);
	}

	//--------------------------------------------------------------
	//  �e�N�X�`���̃��[�h�i�󂫔ԍ������j
	//--------------------------------------------------------------
	int TextureManager::load(ID3D11Device* device, const wchar_t* fileName, size_t maxInstance)
	{
		for (size_t i = 0; i < maxNum; i++)
		{
			if (texture[i].isEmpty())
			{
				load(device, i, fileName, maxInstance);
				return i;
			}
		}
		return -1;
	}

	//--------------------------------------------------------------
	//  �e�N�X�`���̈ꊇ���[�h
	//--------------------------------------------------------------
	void TextureManager::load(ID3D11Device* device, const LoadTexture* data)
	{
		while ((size_t)data->texNum < maxNum)
		{
			release(data->texNum);
			load(device, data->texNum, data->fileName, data->maxInstance);
			data++;
		}
	}

	//--------------------------------------------------------------
	//  �e�N�X�`������i�P�j
	//--------------------------------------------------------------
	void TextureManager::release(int i)
	{
		if ((size_t)i < maxNum)
		{
			texture[i].release();
		}
	}

	//--------------------------------------------------------------
	//  �e�N�X�`���̈ꊇ���
	//--------------------------------------------------------------
	void TextureManager::release(const LoadTexture* data)
	{
		while ((size_t)data->texNum < maxNum)
		{
			release(data->texNum);
			data++;
		}
	}

	//--------------------------------------------------------------
	//  �e�N�X�`������i���ׂāj
	//---------------------- ----------------------------------------
	void TextureManager::releaseAll()
	{
		for (size_t i = 0; i < maxNum; i++) release(i);
	}

	//--------------------------------------------------------------
	//  �`��̑O����
	//--------------------------------------------------------------
	void TextureManager::begin(ID3D11DeviceContext* context, int texNo)
	{
		texture[texNo].begin(context);
	}

	//--------------------------------------------------------------
	//  �`��̌㏈��
	//--------------------------------------------------------------
	void TextureManager::end(ID3D11DeviceContext* context, int texNo)
	{
		texture[texNo].end(context);
	}

	//--------------------------------------------------------------
	//  �e�N�X�`���`��
	//--------------------------------------------------------------
	void TextureManager::draw(int i,
		const VECTOR2& position, const VECTOR2& scale,
		const VECTOR2& texPos, const VECTOR2& texSize,
		const VECTOR2& center, float angle, const VECTOR4& color,
		bool world)
	{
		if ((size_t)i < maxNum)
		{
			texture[i].draw(
				position, scale,
				texPos, texSize,
				center, angle,
				color,
				world);
		}
	}

	//--------------------------------------------------------------
	//  �e�N�X�`���`��
	//--------------------------------------------------------------
	void TextureManager::draw(int i,
		float x, float y,
		float scaleX, float scaleY,
		float left, float top,
		float width, float height,
		float centerX, float centerY, float angle,
		float r, float g, float b, float a,
		bool world)
	{
		if ((size_t)i < maxNum)
		{
			draw(i,
				VECTOR2(x, y), VECTOR2(scaleX, scaleY),
				VECTOR2(left, top), VECTOR2(width, height),
				VECTOR2(centerX, centerY), angle,
				VECTOR4(r, g, b, a),
				world);
		}
	}

	//==========================================================================
	//
	//      �e�N�X�`���N���X
	//
	//==========================================================================

	//--------------------------------------------------------------
	//  �R���X�g���N�^
	//--------------------------------------------------------------
	Texture::Texture() : sprBat(nullptr)
	{
	}

	//--------------------------------------------------------------
	//  �f�X�g���N�^
	//--------------------------------------------------------------
	Texture::~Texture()
	{
		release();
	}

	//--------------------------------------------------------------
	//  �e�N�X�`���̃��[�h
	//--------------------------------------------------------------
	bool Texture::load(ID3D11Device* device, const wchar_t* fileName, size_t maxInstance)
	{
		sprBat = new sprite_batch(device, fileName, maxInstance);
		return (sprBat != nullptr);
	}

	//--------------------------------------------------------------
	//  �`��̑O����
	//--------------------------------------------------------------
	void Texture::begin(ID3D11DeviceContext* context)
	{
		sprBat->begin(context);
	}

	//--------------------------------------------------------------
	//  �`��̌㏈��
	//--------------------------------------------------------------
	void Texture::end(ID3D11DeviceContext* context)
	{
		sprBat->end(context);
	}

	//--------------------------------------------------------------
	//  �e�N�X�`���̕`��
	//--------------------------------------------------------------
	void Texture::draw(
		float x, float y, float scaleX, float scaleY,
		float left, float top, float width, float height,
		float centerX, float centerY, float angle,
		float r, float g, float b, float a,
		bool world)
	{
		draw(VECTOR2(x, y), VECTOR2(scaleX, scaleY),
			VECTOR2(left, top), VECTOR2(width, height),
			VECTOR2(centerX, centerY), angle,
			VECTOR4(r, g, b, a),
			world);
	}

	//--------------------------------------------------------------
	//  �e�N�X�`���̕`��
	//--------------------------------------------------------------
	void Texture::draw(
		const VECTOR2& pos, const VECTOR2& scale,
		const VECTOR2& texPos, const VECTOR2& texSize,
		const VECTOR2& center, float angle,
		const VECTOR4& color,
		bool world)
	{
		if (!sprBat) return;

		if (world)
		{
			float scaleFactor = view::getScale();
			VECTOR2 s = scale * scaleFactor;
			VECTOR2 c = center;
			VECTOR2 v = view::worldToScreen(pos);
			sprBat->render(v, s, texPos, texSize, c, angle, color);
		}
		else
		{
			sprBat->render(
				pos, scale, texPos, texSize,
				center, angle, color);
		}
	}

	//--------------------------------------------------------------
	//  �e�N�X�`���̉��
	//--------------------------------------------------------------
	void Texture::release()
	{
		safe_delete(sprBat);
	}

	//--------------------------------------------------------------
	//  �e�N�X�`���̗L��
	//--------------------------------------------------------------
	bool Texture::isEmpty()
	{
		return (sprBat == nullptr);
	}
}

