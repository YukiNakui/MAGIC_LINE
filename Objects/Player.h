#pragma once
#include "../Engine/GameObject.h"
#include "../Functions/CDTimer.h"
#include "Line.h"
#include "../UI/StartCountdownManager.h"
#include "../Functions/Effect.h"
#include "../Functions/CameraOrbit.h"
#include <vector>
#include "../Engine/VFX.h"

//プレイヤークラス
class Player : public GameObject
{
    //モデルやサウンドなどのリソースハンドル
    int hModel_;
    int hLineSound_;         //線を引く際のサウンド
    int hLineDeleteSound_;   //線を削除する際のサウンド
    int hBGM_;               //プレイヤー専用のBGM

    //プレイヤーの状態
    enum State {
        sBeforeStart,  //開始前
        sMove,         //移動中
        sMoveFinish,   //移動終了
        sResult        //結果表示
    };
    State state_;  //現在のプレイヤーの状態
public:
    Player(GameObject* parent);
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;


    // シャドウマップ描画
    void DrawShadowMap(const XMMATRIX& lightViewProj) override;


    //プレイヤーの状態を取得
    State GetPlayerState() { return state_; }

    //プレイヤーが移動を完了したかどうかを取得
    bool IsPlayerMoveFinished() { return state_ >= sMoveFinish; }

    //現在画面上に存在するカプセルリストを取得
    std::vector<Line*> GetCapsuleList() { return capsuleList_; }

private:
    //各状態の更新メソッド
    void StartUpdate();       //開始前の更新処理
    void MoveUpdate();        //移動中の更新処理
    void MoveFinishUpdate();  //移動終了時の更新処理
    void ResultUpdate();      //結果表示時の更新処理

    //タイマー関連
    CDTimer* cdTimer_;           //共通タイマー
    CDTimer* cupsuleTimer_;      //カプセル生成用タイマー
    float capsuleSpawnInterval_; //カプセル生成間隔

	//プレイヤーの位置・回転関連
    XMMATRIX rotX;              //X軸回転行列
    XMMATRIX rotY;              //Y軸回転行列
    XMVECTOR move;              //移動ベクトル
    XMVECTOR rotVec;            //回転ベクトル
    XMFLOAT3 defaultPlayerPos_; //プレイヤーの初期位置
	void ProcessPlayerRotation(float deltaTime); //プレイヤーの回転処理

	//プレイヤーの移動関連
	void ConstrainPlayerMovement(XMFLOAT3& position); //プレイヤーの移動を制限
	void CheckCollisionWithBall(XMVECTOR& pos); //ボールとの衝突チェック
	void AdjustPlayerOnGround(); //地面に合わせてプレイヤーを調整
	void MovePlayerOutOfScreen(float deltaTime); //画面外に移動する処理

	void HandlePlayerActions(float deltaTime, XMVECTOR& addMove); //プレイヤーのアクション処理
	void HandleEndOfMovement(); //移動終了時の処理
	void ApplyVibration(); //振動を適用

    //プレイヤーの視点・カメラ関連
    XMFLOAT3 lookTarget_;       //カメラの視点ターゲット
    XMVECTOR cameraTargetVec;   //カメラのターゲットベクトル
    XMFLOAT3 targetPos;         //ターゲット位置
    XMVECTOR cameraEyeVec;      //カメラの視点ベクトル
    XMFLOAT3 camPos;            //カメラ位置
    CameraOrbit* pCameraOrbit_; //カメラ軌道
	void SetCameraTarget(XMVECTOR targetVec); //カメラのターゲットを設定
	void SetCameraPos(XMVECTOR camVec);    //カメラの位置を設定

    //線（カプセル）関連
    float maxLineValue_;             //引ける線の最大値
    float currentLineValue_;         //現在引いた線の量
    Line* pCapsule_;                 //カプセル（線）オブジェクト
    std::vector<Line*> capsuleList_; //現在画面上に存在するカプセルリスト
    std::vector<Line*> capsulePool_; //再利用用のカプセルプール
    Line* GetCapsuleFromPool();      //カプセルをプールから取得
    void ClearCapsules();            //カプセルをクリア
	void LineGaugeUpdate();          //線の更新処理

    //カウントダウン関連
    StartCountdownManager* pStartCountdownManager_; //カウントダウンマネージャー

    //プレイヤーの操作状態
    bool isMoveStarted_; //プレイヤーが動き始めたか
    bool canControl_;    //プレイヤーが操作できるかどうか

    //プレイヤーの移動範囲制限
    bool CheckPlayerOutOfRange(XMFLOAT3 playerPos, XMFLOAT3 maxPos, XMFLOAT3 minPos); //範囲外チェック

    //その他の状態
    float vibrationTime_;  //振動時間
    bool isInvisible_;     //プレイヤーが透明かどうか
    bool isPlayerHitting_; //プレイヤーが衝突しているかどうか

    //エフェクト関連
    XMFLOAT3 rocketNozzlePos_;    //ロケットノズルの位置
    XMVECTOR fireEffectVec_;      //炎のエフェクトベクトル
    EmitterData fireEffectData_;  //炎エフェクトのデータ
    int hFireEffect_;             //炎エフェクトのハンドル
    EmitterData sparkEffectData_; //スパークエフェクトのデータ
    int hSparkEffect_;            //スパークエフェクトのハンドル
    Effect* fireEffect_;          //炎エフェクト
    Effect* sparkEffect_;         //スパークエフェクト
	void SetFireEffect(); //炎エフェクトの設定
	void SetSparkEffect(); //スパークエフェクトの設定
	void UpdateEffects(const XMMATRIX& rotX, const XMMATRIX& rotY); //エフェクトの更新
};
