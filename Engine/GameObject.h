#pragma once
#include <DirectXMath.h>
#include <list>
#include <string>
#include <assert.h>
#include <algorithm>
#include "SphereCollider.h"
#include "BoxCollider.h"
#include "CapsuleCollider.h"
#include "Transform.h"




using namespace DirectX;


//-----------------------------------------------------------
//全てのゲームオブジェクト（シーンも含めて）が継承するインターフェース
// ゲームオブジェクトは、親子構造になっていて、
// マトリクスの影響を受けることになる
//-----------------------------------------------------------
class GameObject
{
protected:
	
	//位置や向きなどを管理するオブジェクト
	Transform				transform_;

	//オブジェクトの名前
	std::string				objectName_;

	//衝突判定リスト
	std::list<Collider*>	colliderList_;	

	int renderOrder_; // 描画順序
public:
	//コンストラクタ
	GameObject();
	GameObject(GameObject* parent);
	GameObject(GameObject* parent, const std::string& name);

	//デストラクタ
	virtual ~GameObject();

	//各オブジェクトで必ず作る関数
	virtual void Initialize(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw() = 0;
	virtual void Release(void) = 0;

	//自分の該当関数を読んだ後、子供の関数も呼ぶ
	void UpdateSub();
	void DrawSub();
	void ReleaseSub();


	////ローカル行列の取得（このオブジェクトの行列）
	////戻値：ローカル行列
	//XMMATRIX GetLocalMatrix();

	//ワールド行列の取得（親の影響を受けた最終的な行列）
	//戻値：ワールド行列
	XMMATRIX GetWorldMatrix();



	//各フラグの制御
	bool IsDead();			// 削除するかどうか
	void KillMe();			// 自分を削除する
	void Enter();			// Updateを許可
	void Leave();			// Updateを拒否
	void Visible();			// Drawを許可
	void Invisible();		// Drawを拒否
	bool IsInitialized();	// 初期化済みかどうか
	void SetInitialized();	// 初期化済みにする
	bool IsEntered();		// Update実行していいか
	bool IsVisibled();		// Draw実行していいか


	//子オブジェクトリストを取得
	//戻値：子オブジェクトリスト
	std::list<GameObject*>* GetChildList();

	//親オブジェクトを取得
	//戻値：親オブジェクトのアドレス
	GameObject* GetParent();

	//名前でオブジェクトを検索（対象は自分の子供以下）
	//引数：name	検索する名前
	//戻値：見つけたオブジェクトのアドレス（見つからなければnullptr）
	GameObject* FindChildObject(const std::string& name);

	/// <summary>
	/// クラス名で検索できるもの
	/// </summary>
	template<class C>
	C* FindGameObject() {
		auto list = GetChildList();
		for (GameObject* obj : *list) {
			C* ret = dynamic_cast<C*>(obj);
			if (ret != nullptr) {
				return ret;
			}
		}
		return nullptr;
	}

	template<class C>
	std::list<C*> FindGameObjects() {
		std::list<C*> rets;
		auto list = GetChildList();
		for (GameObject* obj : *list) {
			C* ret = dynamic_cast<C*>(obj);
			if (ret != nullptr) {
				rets.push_back(ret);
			}
		}
		return rets;
	}

	/// <summary>
	/// クラス名で検索できるもの
	/// </summary>
	template<class C>
	C* FindGameObject(const std::string& tag) {
		auto list = GetChildList();
		for (GameObject* obj : *list) {
			C* ret = dynamic_cast<C*>(obj);
			if (ret != nullptr) {
				if (obj->GetObjectName() == tag) {
					return ret;
				}
			}
		}
		return nullptr;
	}

	template<class C>
	std::list<C*> FindGameObjects(const std::string& tag) {
		std::list<C*> rets;
		auto list = GetChildList();
		for (GameObject* obj : *list) {
			C* ret = dynamic_cast<C*>(obj);
			if (ret != nullptr) {
				if (obj->GetObjectName() == tag) {
					rets.push_back(ret);
				}
			}
		}
		return rets;
	}

	//名前でオブジェクトを検索（対象は全体）
	//引数：検索する名前
	//戻値：見つけたオブジェクトのアドレス
	GameObject* FindObject(const std::string& name) { return GetRootJob()->FindChildObject(name); }

	//オブジェクトの名前を取得
	//戻値：名前
	const std::string& GetObjectName(void) const;

	//子オブジェクトを追加（リストの最後へ）
	//引数：追加するオブジェクト
	void PushBackChild(GameObject* obj);

	//子オブジェクトを追加（リストの先頭へ）
	//引数：obj 追加するオブジェクト
	void PushFrontChild(GameObject* obj);

	//子オブジェクトを全て削除
	void KillAllChildren();



	//コライダー（衝突判定）を追加する
	void AddCollider(Collider * collider);

	//何かと衝突した場合に呼ばれる（オーバーライド用）
	//引数：pTarget	衝突した相手
	virtual void OnCollision(GameObject* pTarget) {};

	//コライダー（衝突判定）を削除
	void ClearCollider();

	//衝突判定
	//引数：pTarget	衝突してるか調べる相手
	void Collision(GameObject* pTarget);

	//テスト用の衝突判定枠を表示
	void CollisionDraw();

	//RootJobを取得
	GameObject* GetRootJob();


	//各アクセス関数
	XMFLOAT3 GetPosition() { return transform_.position_; }
	XMFLOAT3 GetRotate() { return transform_.rotate_; }
	XMFLOAT3 GetScale() { return transform_.scale_; }
	XMFLOAT3 GetWorldPosition() { return Transform::Float3Add(GetParent()->transform_.position_ , transform_.position_); }
	XMFLOAT3 GetWorldRotate() { return Transform::Float3Add(GetParent()->transform_.rotate_, transform_.rotate_); }
	XMFLOAT3 GetWorldScale() { return Transform::Float3Add(GetParent()->transform_.scale_, transform_.scale_); }
	void SetPosition(XMFLOAT3 position) { transform_.position_ = position; }
	void SetPosition(float x, float y, float z) { SetPosition(XMFLOAT3( x, y, z )); }
	void SetRotate(XMFLOAT3 rotate) { transform_.rotate_ = rotate; }
	void SetRotate(float x, float y, float z) { SetRotate(XMFLOAT3( x, y, z )); }
	void SetRotateX(float x) { SetRotate(x, transform_.rotate_.y, transform_.rotate_.z); }
	void SetRotateY(float y) { SetRotate(transform_.rotate_.x, y, transform_.rotate_.z); }
	void SetRotateZ(float z) { SetRotate(transform_.rotate_.x, transform_.rotate_.y, z); }
	void SetScale(XMFLOAT3 scale) { transform_.scale_ = scale; }
	void SetScale(float x, float y, float z) { SetScale(XMFLOAT3(x, y, z)); }
	void SetTransformFloat3(XMFLOAT3 position,XMFLOAT3 rotate,XMFLOAT3 scale) { 
		transform_.position_ = position;
		transform_.rotate_ = rotate;
		transform_.scale_ = scale;
	}


	void SetRenderOrder(int order) { renderOrder_ = order; }
	int GetRenderOrder() const { return renderOrder_; }

	void AddChild(GameObject* child) { childList_.push_back(child); }

	//子オブジェクトの描画順をソート（描画順が大きいものから描画される）
	void SortChildrenByRenderOrder() {
		childList_.sort([](GameObject* a, GameObject* b) {
			return a->GetRenderOrder() > b->GetRenderOrder();
			});
	}

	void DrawChildren() {
		SortChildrenByRenderOrder();
		for (auto& child : childList_) {
			child->Draw();
		}
	}




	// 追加: シャドウマップ用の描画関数
	virtual void SetLightViewProj(const XMMATRIX&) {}
	
	// シャドウマップ描画（追加！）
	virtual void DrawShadowMap(const DirectX::XMMATRIX& lightViewProj) {}

	void DrawShadowMapSub(const DirectX::XMMATRIX& lightViewProj)
	{
		DrawShadowMap(lightViewProj);
		for (auto it = childList_.begin(); it != childList_.end(); ++it)
		{
			(*it)->DrawShadowMapSub(lightViewProj);
		}
	}


private:

	//オブジェクト削除（再帰）
	//引数：obj　削除するオブジェクト
	void KillObjectSub(GameObject* obj);


private:
	//フラグ
	struct OBJECT_STATE
	{
		unsigned initialized : 1;	//初期化済みか
		unsigned entered : 1;		//更新するか
		unsigned visible : 1;		//描画するか
		unsigned dead : 1;			//削除するか
	};
	OBJECT_STATE state_;

	//親オブジェクト
	GameObject* pParent_;

	//子オブジェクトリスト
	std::list<GameObject*> childList_;
};


//オブジェクトを作成するテンプレート
template <class T>
T* Instantiate(GameObject* pParent)
{
	T* pNewObject = new T(pParent);
	if (pParent != nullptr)
	{
		pParent->PushBackChild(pNewObject);
	}
	pNewObject->Initialize();
	return pNewObject;
}

// オーバーロード版
template <class T, class... Args>
T* InstantiateEx(GameObject* pParent, Args&&... args)
{
	T* pNewObject = new T(pParent, std::forward<Args>(args)...);
	if (pParent != nullptr)
	{
		pParent->PushBackChild(pNewObject);
	}
	pNewObject->Initialize();
	return pNewObject;
}
