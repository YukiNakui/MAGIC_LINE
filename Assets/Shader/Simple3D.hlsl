//������������������������������������������������������������������������������
 // �e�N�X�`�����T���v���[�f�[�^�̃O���[�o���ϐ���`
//������������������������������������������������������������������������������
Texture2D		g_texture: register(t0);	//�e�N�X�`���[
SamplerState	g_sampler : register(s0);	//�T���v���[

Texture2D g_shadowMap : register(t1); // �ǉ�
SamplerState g_shadowSampler : register(s1); // �ǉ�

//������������������������������������������������������������������������������
 // �R���X�^���g�o�b�t�@
// DirectX �����瑗�M����Ă���A�|���S�����_�ȊO�̏����̒�`
//������������������������������������������������������������������������������
cbuffer global
{
	float4x4	g_matWVP;			// ���[���h�E�r���[�E�v���W�F�N�V�����̍����s��
	float4x4	g_matNormalTrans;	// �@���̕ϊ��s��i��]�s��Ɗg��̋t�s��j
	float4x4	g_matWorld;			// ���[���h�ϊ��s��
	float4		g_vecLightDir;		// ���C�g�̕����x�N�g��
	float4		g_vecDiffuse;		// �f�B�t���[�Y�J���[�i�}�e���A���̐F�j
	float4		g_vecAmbient;		// �A���r�G���g�J���[�i�e�̐F�j
	float4		g_vecSpeculer;		// �X�y�L�����[�J���[�i�n�C���C�g�̐F�j
	float4		g_vecCameraPosition;// ���_�i�J�����̈ʒu�j
	float		g_shuniness;		// �n�C���C�g�̋����i�e�J����j
	bool		g_isTexture;		// �e�N�X�`���\���Ă��邩�ǂ���
    float4x4 g_matLightViewProj; // �ǉ�
};

//������������������������������������������������������������������������������
// ���_�V�F�[�_�[�o�́��s�N�Z���V�F�[�_�[���̓f�[�^�\����
//������������������������������������������������������������������������������
struct VS_OUT
{
	float4 pos    : SV_POSITION;	//�ʒu
	float4 normal : TEXCOORD2;		//�@��
	float2 uv	  : TEXCOORD0;		//UV���W
	float4 eye	  : TEXCOORD1;		//����
    float4 shadowPos : TEXCOORD3; // �ǉ�
};

//������������������������������������������������������������������������������
// ���_�V�F�[�_
//������������������������������������������������������������������������������
VS_OUT VS(float4 pos : POSITION, float4 Normal : NORMAL, float2 Uv : TEXCOORD)
{
	//�s�N�Z���V�F�[�_�[�֓n�����
	VS_OUT outData;

	//���[�J�����W�ɁA���[���h�E�r���[�E�v���W�F�N�V�����s���������
	//�X�N���[�����W�ɕϊ����A�s�N�Z���V�F�[�_�[��
	outData.pos = mul(pos, g_matWVP);		

	//�@���̕ό`
	Normal.w = 0;					//4�����ڂ͎g��Ȃ��̂�0
	Normal = mul(Normal, g_matNormalTrans);		//�I�u�W�F�N�g���ό`����Ζ@�����ό`
	outData.normal = Normal;		//������s�N�Z���V�F�[�_�[��

	//�����x�N�g���i�n�C���C�g�̌v�Z�ɕK�v
	float4 worldPos = mul(pos, g_matWorld);					//���[�J�����W�Ƀ��[���h�s��������ă��[���h���W��
	outData.eye = normalize(g_vecCameraPosition - worldPos);	//���_���璸�_�ʒu�������Z�����������߂ăs�N�Z���V�F�[�_�[��

	//UV�u���W
	outData.uv = Uv;	//���̂܂܃s�N�Z���V�F�[�_�[��

	
    outData.shadowPos = mul(worldPos, g_matLightViewProj); // �ǉ�

	//�܂Ƃ߂ďo��
	return outData;
}

//������������������������������������������������������������������������������
// �s�N�Z���V�F�[�_
//������������������������������������������������������������������������������
float4 PS(VS_OUT inData) : SV_Target
{
    #if 0
	//���C�g�̌���
    float4 lightDir = g_vecLightDir; //�O���[�o���ϐ��͕ύX�ł��Ȃ��̂ŁA�������񃍁[�J���ϐ���
    lightDir = normalize(lightDir); //�����������K�v�Ȃ̂Ő��K��

	//�@���̓s�N�Z���V�F�[�_�[�Ɏ����Ă������_�ŕ⊮���꒷�����ς���Ă���
	//���K�����Ă����Ȃ��Ɩʂ̖��邳�����������Ȃ�
    inData.normal = normalize(inData.normal);

	//�g�U���ˌ��i�f�B�t���[�Y�j
	//�@���ƌ��̃x�N�g���̓��ς��A�����̖��邳�ɂȂ�
    float4 shade = saturate(dot(inData.normal, -lightDir));
    shade.a = 1; //�Â��Ƃ��낪�����ɂȂ�̂ŁA�����I�ɃA���t�@��1

    float4 diffuse;
	//�e�N�X�`���L��
    if (g_isTexture == true)
    {
		//�e�N�X�`���̐F
        diffuse = g_texture.Sample(g_sampler, inData.uv);
    }
    else
    {
		//�}�e���A���̐F
        diffuse = g_vecDiffuse;
    }

	//�����i�A���r�G���g�j
	//�����Maya���Ŏw�肵�A�O���[�o���ϐ��Ŏ󂯎�������̂����̂܂�
    float4 ambient = g_vecAmbient;
    ambient = float4(0.6f, 0.6f, 0.6f, 0.6f);
	//���ʔ��ˌ��i�X�y�L�����[�j
    float4 speculer = float4(0, 0, 0, 0); //�Ƃ肠�����n�C���C�g�͖����ɂ��Ă����āc
    if (g_vecSpeculer.a != 0)	//�X�y�L�����[�̏�񂪂����
    {
        float4 R = reflect(lightDir, inData.normal); //�����˃x�N�g��
        speculer = pow(saturate(dot(R, inData.eye)), g_shuniness) * g_vecSpeculer; //�n�C���C�g�����߂�
    }

	//�ŏI�I�ȐF
    return diffuse * shade + diffuse * ambient + speculer;
	#else
	
    // �V���h�E�}�b�v���W�v�Z
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
        float shadowDepth = g_shadowMap.Sample(g_shadowSampler, shadowCoord.xy).r;
        float currentDepth = saturate(shadowCoord.z);
        float bias = 0.005f;
        shadow = (currentDepth > shadowDepth + bias) ? 0.4f : 1.0f;
    }

    // �ʏ�̃��C�e�B���O�v�Z
    float4 lightDir = normalize(g_vecLightDir);
    inData.normal = normalize(inData.normal);
    float shade = saturate(dot(inData.normal, -lightDir));

    float4 diffuse;
    if (g_isTexture)
        diffuse = g_texture.Sample(g_sampler, inData.uv);
    else
        diffuse = g_vecDiffuse;
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
    #endif
}