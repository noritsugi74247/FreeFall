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


//	CSOファイル読み込み関数
bool	LoadCSOFile(BYTE** data, long* size, const char* filename)
{
	FILE* fp;
	//	ファイルオープン
	if (fopen_s(&fp, filename, "rb"))	return false;

	//	ファイルサイズ取得
	fseek(fp, 0, SEEK_END);		//	ファイルポインタを終端に移動
	long sz = ftell(fp);		//	終端位置の保存 = ファイルサイズ
	fseek(fp, 0, SEEK_SET);		//	ファイルポインタを最初に戻す

								//	ファイル読み込み用領域の確保
	*data = new BYTE[sz];
	//	ファイル読み込み
	size_t s = fread_s(*data, sizeof(BYTE) * sz,
		sizeof(BYTE), sz, fp);

	//	読み込み終了
	fclose(fp);

	//	エラーチェック
	if (s != sizeof(BYTE) * sz)	return false;

	*size = sz;			//	ファイルサイズ保存
	return	true;
}



bool ResourceManager::LoadShaderResourceView(ID3D11Device* device, const wchar_t* filename,
	ID3D11ShaderResourceView** srv, D3D11_TEXTURE2D_DESC** texDesc)
{
	//	エラーチェック
	if (!device)		return false;
	if (!filename)		return false;
	if (*srv)			return false;

	HRESULT hr;
	//	ID3D11Resource* resource = nullptr;;
	ShaderResourceViewData* find = nullptr;
	int no = -1;

	//	対象のファイルが既に存在しているかを検索
	for (int n = 0; n < RESOURCE_MAX; n++)
	{
		ShaderResourceViewData* p = &srvData[n];

		//	データの有無確認
		if (p->count <= 0)
		{
			if (no == -1)		no = n;
			continue;
		}

		//	文字列比較
		if (wcscmp(p->filename, filename) != 0)	continue;

		//	一致したから読み込んでる => 発見!
		find = p;

		break;
	}

	//	ファイルが見つからなかったら新規読み込み
	if (!find)
	{
		if (no == -1)	return	false;		//	空きが無い

		find = &srvData[no];
		//	テクスチャ読み込み
		ID3D11Resource* resource = nullptr;
		hr = DirectX::CreateWICTextureFromFile(device, filename, &resource,
			&find->shader_resource_view);
		//		→データが読み込めなかったら return false;
		if (FAILED(hr))	return false;

		//	texture2d_descの取得
		ID3D11Texture2D* texture2d;
		hr = resource->QueryInterface(&texture2d);
		//		→データが読み込めなかったら return false;
		if (FAILED(hr))
		{
			resource->Release();
			find->shader_resource_view->Release();
			return false;
		}
		//	texture2d_desc保存
		texture2d->GetDesc(&find->texDesc);

		//	ポインタ変数 解放
		texture2d->Release();
		resource->Release();

		//	ファイル名のコピー
		wcscpy_s(find->filename, 256, filename);
	}

	//////////////////////////
	//	共通部
	//////////////////////////

	//	データを返す
	*srv = find->shader_resource_view;	//	ShaderResourceView保存
	*texDesc = &find->texDesc;				//	texture2d_desc保存

	//	保持数増加
	find->count++;


	return	true;
}

void ResourceManager::ReleaseShaderResourceView(ID3D11ShaderResourceView* srv)
{
	if (!srv)			return;		//	検索対象Unknown　終了します

	//	Targetの検索開始
	for (int n = 0; n < RESOURCE_MAX; n++)
	{
		ShaderResourceViewData* p = &srvData[n];

		//	未使用データなら、次のデータへ移行します
		if (p->count == 0)	continue;
		//	検索対象と不一致なら、次のデータへ移行します
		if (p->shader_resource_view != srv)	continue;

		//	Targetを発見 Releaseします
		p->Release();
		break;
	}
}


bool ResourceManager::LoadVertexShaders(ID3D11Device* device, const char* filename,
	D3D11_INPUT_ELEMENT_DESC* elementDescs, int numElement,
	ID3D11VertexShader** vs, ID3D11InputLayout** il)
{
	//	エラーチェック
	if (!device)		return false;
	if (!filename)		return false;
	if (*vs)			return false;
	if (*il)			return false;

	HRESULT hr;
	VertexShadersData* find = nullptr;
	int no = -1;

	//	対象のファイルが既に存在しているかを検索
	for (int n = 0; n < RESOURCE_MAX; n++)
	{
		VertexShadersData* p = &vsData[n];

		//	データの有無確認
		if (p->count == 0)
		{
			if (no == -1)	no = n;
			continue;
		}
		//	文字列比較
		if (strcmp(p->filename, filename) != 0)	continue;

		//	一致したから読み込んでる => 発見!
		find = p;
		//	ついでに作るものがあればここで
		//		→ VertexShadersの場合、特に無し
		break;
	}

	//	ファイルが見つからなかったら新規読み込み
	if (!find)
	{
		if (no == -1)	return	false;		//	空きが無い

		find = &vsData[no];
		//	新規読み込み
		BYTE* shader_data;
		long				size;
		LoadCSOFile(&shader_data, &size, filename);
		hr = device->CreateVertexShader(shader_data, size, nullptr, &find->vertex_shader);
		//		→データが読み込めなかったら return false;
		if (FAILED(hr))	return	false;

		hr = device->CreateInputLayout(elementDescs, numElement,
			shader_data, size, &find->input_layout);
		delete[] shader_data;
		//		→データが読み込めなかったら return false;
		if (FAILED(hr))
		{
			find->vertex_shader->Release();
			return false;
		}

		//	ファイル名のコピー
		strcpy_s(find->filename, 256, filename);
	}

	//////////////////////////
	//	共通部
	//////////////////////////

	//	共通して作るものがあればここで作る
	//		→VertexShadersで特にやる事は無し

	//	データを返す
	*vs = find->vertex_shader;
	*il = find->input_layout;

	//	保持数増加
	find->count++;

	return	true;
}

void ResourceManager::ReleaseVertexShaders(ID3D11VertexShader* vs, ID3D11InputLayout* il)
{
	if (!vs)			return;		//	検索対象Unknown　終了します
	if (!il)			return;		//	検索対象Unknown　終了します

	//	Targetの検索開始
	for (int n = 0; n < RESOURCE_MAX; n++)
	{
		VertexShadersData* p = &vsData[n];

		//	未使用データなら、次のデータへ移行します
		if (p->count == 0)	continue;
		//	検索対象と不一致なら、次のデータへ移行します
		if (p->vertex_shader != vs)	continue;
		if (p->input_layout != il)	continue;

		//	Targetを発見 Releaseします
		p->Release();
		break;
	}
}



bool ResourceManager::LoadPixelShaders(ID3D11Device* device, const char* filename, ID3D11PixelShader** ps)
{
	//	エラーチェック
	if (!device)		return false;
	if (!filename)		return false;
	if (*ps)			return false;

	HRESULT hr;
	PixelShadersData* find = nullptr;
	int no = -1;

	//	対象のファイルが既に存在しているかを検索
	for (int n = 0; n < RESOURCE_MAX; n++)
	{
		PixelShadersData* p = &psData[n];

		//	データの有無確認
		if (p->count == 0)
		{
			if (no == -1)	no = n;
			continue;
		}
		//	文字列比較
		if (strcmp(p->filename, filename) != 0)	continue;

		//	一致したから読み込んでる => 発見!
		find = p;
		//	ついでに作るものがあればここで
		//		→ VertexShadersの場合、特に無し
		break;
	}

	//	ファイルが見つからなかったら新規読み込み
	if (!find)
	{
		if (no == -1)	return	false;		//	空きが無い

		find = &psData[no];
		//	新規読み込み
		BYTE* shader_data;
		long				size;
		LoadCSOFile(&shader_data, &size, filename);
		hr = device->CreatePixelShader(shader_data, size, nullptr, &find->pixel_shader);
		//		→データが読み込めなかったら return false;
		if (FAILED(hr))	return	false;

		//	ファイル名のコピー
		strcpy_s(find->filename, 256, filename);

		delete[]	shader_data;
	}

	//////////////////////////
	//	共通部
	//////////////////////////

	//	共通して作るものがあればここで作る
	//		→VertexShadersで特にやる事は無し

	//	データを返す
	*ps = find->pixel_shader;

	//	保持数増加
	find->count++;

	return	true;
}

void ResourceManager::ReleasePixelShaders(ID3D11PixelShader* ps)
{
	if (!ps)			return;		//	検索対象Unknown　終了します

									//	Targetの検索開始
	for (int n = 0; n < RESOURCE_MAX; n++)
	{
		PixelShadersData* p = &psData[n];

		//	未使用データなら、次のデータへ移行します
		if (p->count == 0)	continue;
		//	検索対象と不一致なら、次のデータへ移行します
		if (p->pixel_shader != ps)	continue;

		//	Targetを発見 Releaseします
		p->Release();
		break;
	}
}





namespace mylib
{

	//==========================================================================
	//
	//      テクスチャ管理クラス
	//
	//==========================================================================

	//--------------------------------------------------------------
	//  コンストラクタ
	//--------------------------------------------------------------
	TextureManager::TextureManager() : maxNum(0), texture(nullptr), current(0)
	{
		init(system::TEXTURE_NUM);
	}

	//--------------------------------------------------------------
	//  デストラクタ
	//--------------------------------------------------------------
	TextureManager::~TextureManager()
	{
		uninit();
	}

	//--------------------------------------------------------------
	//  初期化
	//--------------------------------------------------------------
	void TextureManager::init(size_t max)
	{
		uninit();	// 念のため

		texture = new Texture[max];
		maxNum = max;
	}

	//--------------------------------------------------------------
	//  終了処理
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
	//  テクスチャロード（番号指定）
	//--------------------------------------------------------------
	void TextureManager::load(ID3D11Device* device, int i, const wchar_t* fileName, size_t maxInstance)
	{
		if ((size_t)i < maxNum) texture[i].load(device, fileName, maxInstance);
	}

	//--------------------------------------------------------------
	//  テクスチャのロード（空き番号検索）
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
	//  テクスチャの一括ロード
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
	//  テクスチャ解放（１つ）
	//--------------------------------------------------------------
	void TextureManager::release(int i)
	{
		if ((size_t)i < maxNum)
		{
			texture[i].release();
		}
	}

	//--------------------------------------------------------------
	//  テクスチャの一括解放
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
	//  テクスチャ解放（すべて）
	//---------------------- ----------------------------------------
	void TextureManager::releaseAll()
	{
		for (size_t i = 0; i < maxNum; i++) release(i);
	}

	//--------------------------------------------------------------
	//  描画の前処理
	//--------------------------------------------------------------
	void TextureManager::begin(ID3D11DeviceContext* context, int texNo)
	{
		texture[texNo].begin(context);
	}

	//--------------------------------------------------------------
	//  描画の後処理
	//--------------------------------------------------------------
	void TextureManager::end(ID3D11DeviceContext* context, int texNo)
	{
		texture[texNo].end(context);
	}

	//--------------------------------------------------------------
	//  テクスチャ描画
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
	//  テクスチャ描画
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
	//      テクスチャクラス
	//
	//==========================================================================

	//--------------------------------------------------------------
	//  コンストラクタ
	//--------------------------------------------------------------
	Texture::Texture() : sprBat(nullptr)
	{
	}

	//--------------------------------------------------------------
	//  デストラクタ
	//--------------------------------------------------------------
	Texture::~Texture()
	{
		release();
	}

	//--------------------------------------------------------------
	//  テクスチャのロード
	//--------------------------------------------------------------
	bool Texture::load(ID3D11Device* device, const wchar_t* fileName, size_t maxInstance)
	{
		sprBat = new sprite_batch(device, fileName, maxInstance);
		return (sprBat != nullptr);
	}

	//--------------------------------------------------------------
	//  描画の前処理
	//--------------------------------------------------------------
	void Texture::begin(ID3D11DeviceContext* context)
	{
		sprBat->begin(context);
	}

	//--------------------------------------------------------------
	//  描画の後処理
	//--------------------------------------------------------------
	void Texture::end(ID3D11DeviceContext* context)
	{
		sprBat->end(context);
	}

	//--------------------------------------------------------------
	//  テクスチャの描画
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
	//  テクスチャの描画
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
	//  テクスチャの解放
	//--------------------------------------------------------------
	void Texture::release()
	{
		safe_delete(sprBat);
	}

	//--------------------------------------------------------------
	//  テクスチャの有無
	//--------------------------------------------------------------
	bool Texture::isEmpty()
	{
		return (sprBat == nullptr);
	}
}

