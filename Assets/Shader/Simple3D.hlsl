//───────────────────────────────────────
 // テクスチャ＆サンプラーデータのグローバル変数定義
//───────────────────────────────────────
Texture2D		g_texture: register(t0);	//テクスチャー
SamplerState	g_sampler : register(s0);	//サンプラー

Texture2D g_shadowMap : register(t1); // 追加
SamplerComparisonState g_shadowSampler : register(s1); // 追加

//───────────────────────────────────────
 // コンスタントバッファ
// DirectX 側から送信されてくる、ポリゴン頂点以外の諸情報の定義
//───────────────────────────────────────
cbuffer global
{
	float4x4	g_matWVP;			// ワールド・ビュー・プロジェクションの合成行列
	float4x4	g_matNormalTrans;	// 法線の変換行列（回転行列と拡大の逆行列）
	float4x4	g_matWorld;			// ワールド変換行列
	float4		g_vecLightDir;		// ライトの方向ベクトル
	float4		g_vecDiffuse;		// ディフューズカラー（マテリアルの色）
	float4		g_vecAmbient;		// アンビエントカラー（影の色）
	float4		g_vecSpeculer;		// スペキュラーカラー（ハイライトの色）
	float4		g_vecCameraPosition;// 視点（カメラの位置）
	float		g_shuniness;		// ハイライトの強さ（テカリ具合）
	bool		g_isTexture;		// テクスチャ貼ってあるかどうか
    
    float2 padding; // 追加：パディング（16バイト境界に合わせるため）
    float4x4 g_matLightViewProj; // 追加
};

//───────────────────────────────────────
// 頂点シェーダー出力＆ピクセルシェーダー入力データ構造体
//───────────────────────────────────────
struct VS_OUT
{
	float4 pos    : SV_POSITION;	//位置
	float4 normal : TEXCOORD2;		//法線
	float2 uv	  : TEXCOORD0;		//UV座標
	float4 eye	  : TEXCOORD1;		//視線
    
    float4 shadowPos : TEXCOORD3; // 追加
};

//───────────────────────────────────────
// 頂点シェーダ
//───────────────────────────────────────
VS_OUT VS(float4 pos : POSITION, float4 Normal : NORMAL, float2 Uv : TEXCOORD)
{
	//ピクセルシェーダーへ渡す情報
	VS_OUT outData;

	//ローカル座標に、ワールド・ビュー・プロジェクション行列をかけて
	//スクリーン座標に変換し、ピクセルシェーダーへ
	outData.pos = mul(pos, g_matWVP);		

	//法線の変形
	Normal.w = 0;					//4次元目は使わないので0
	Normal = mul(Normal, g_matNormalTrans);		//オブジェクトが変形すれば法線も変形
	outData.normal = Normal;		//これをピクセルシェーダーへ

	//視線ベクトル（ハイライトの計算に必要
	float4 worldPos = mul(pos, g_matWorld);					//ローカル座標にワールド行列をかけてワールド座標へ
	outData.eye = normalize(g_vecCameraPosition - worldPos);	//視点から頂点位置を引き算し視線を求めてピクセルシェーダーへ

	//UV「座標」
	outData.uv = Uv;	//そのままピクセルシェーダーへ

	
    outData.shadowPos = mul(worldPos, g_matLightViewProj); // 追加

	//まとめて出力
	return outData;
}

//───────────────────────────────────────
// ピクセルシェーダ
//───────────────────────────────────────
float4 PS(VS_OUT inData) : SV_Target
{
    #if 1
	//ライトの向き
    float4 lightDir = g_vecLightDir; //グルーバル変数は変更できないので、いったんローカル変数へ
    lightDir = normalize(lightDir); //向きだけが必要なので正規化

	//法線はピクセルシェーダーに持ってきた時点で補完され長さが変わっている
	//正規化しておかないと面の明るさがおかしくなる
    inData.normal = normalize(inData.normal);

	//拡散反射光（ディフューズ）
	//法線と光のベクトルの内積が、そこの明るさになる
    float4 shade = saturate(dot(inData.normal, -lightDir));
    shade.a = 1; //暗いところが透明になるので、強制的にアルファは1

    float4 diffuse;
	//テクスチャ有無
    if (g_isTexture == true)
    {
		//テクスチャの色
        diffuse = g_texture.Sample(g_sampler, inData.uv);
    }
    else
    {
		//マテリアルの色
        diffuse = g_vecDiffuse;
    }

	//環境光（アンビエント）
	//これはMaya側で指定し、グローバル変数で受け取ったものをそのまま
    float4 ambient = g_vecAmbient;
    ambient = float4(0.6f, 0.6f, 0.6f, 0.6f);
	//鏡面反射光（スペキュラー）
    float4 speculer = float4(0, 0, 0, 0); //とりあえずハイライトは無しにしておいて…
    if (g_vecSpeculer.a != 0)	//スペキュラーの情報があれば
    {
        float4 R = reflect(lightDir, inData.normal); //正反射ベクトル
        speculer = pow(saturate(dot(R, inData.eye)), g_shuniness) * g_vecSpeculer; //ハイライトを求める
    }

	//最終的な色
    return diffuse * shade + diffuse * ambient + speculer;
	#else
	
    // Shadow判定
    float3 shadowCoord = inData.shadowPos.xyz / inData.shadowPos.w;
    shadowCoord = shadowCoord * 0.5f + 0.5f;
    float shadow = 1.0f;
    if (shadowCoord.x < 0 || shadowCoord.x > 1 ||
        shadowCoord.y < 0 || shadowCoord.y > 1 ||
        shadowCoord.z < 0 || shadowCoord.z > 1)
    {
        shadow = 1.0f;
    }
    else
    {
        float bias = 0.0005f;
        shadow = g_shadowMap.SampleCmpLevelZero(g_shadowSampler, shadowCoord.xy, shadowCoord.z - bias);
    }

    // 通常のライティング
    float4 lightDir = normalize(g_vecLightDir);
    inData.normal = normalize(inData.normal);
    float shade = saturate(dot(inData.normal, -lightDir));

    float4 diffuse = g_isTexture
        ? g_texture.Sample(g_sampler, inData.uv)
        : g_vecDiffuse;
    diffuse.a = 1.0f;

    float4 ambient = float4(0.6f, 0.6f, 0.6f, 1.0f);
    float4 speculer = float4(0, 0, 0, 0);
    if (g_vecSpeculer.a != 0)
    {
        float4 R = reflect(lightDir, inData.normal);
        speculer = pow(saturate(dot(R, inData.eye)), g_shuniness) * g_vecSpeculer;
    }

    float4 color = shadow * (diffuse * shade + diffuse * ambient) + speculer;
    color.a = 1.0f;
    return color;
    
    
    //return float4(inData.shadowPos.x, 0, 0, 1); // X成分
    //return float4(0, inData.shadowPos.y, 0, 1); // Y成分
    //return float4(0, 0, inData.shadowPos.z, 1); // Z成分
    //return float4(inData.shadowPos.w, inData.shadowPos.w, inData.shadowPos.w, 1); // W成分
    #endif
}