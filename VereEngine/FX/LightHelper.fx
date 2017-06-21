
struct AmbientLight
{
	float4 ambient;
	float power;
	float pad;
};

struct DirectionalLight
{
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float3 direction;
	float power;
	float pad;
};

struct PointLight
{
	float4 ambient;
	float4 diffuse;
	float4 specular;

	float3 position;
	float range;

	float3 att;
	float power;
	float pad;
};

struct SpotLight
{
	float4 ambient;
	float4 diffuse;
	float4 specular;

	float3 position;
	float range;

	float3 direction;
	float spot;

	float3 att;
	float power;
	float pad;
};

struct Fog
{
	float4 color;
	float minDistance;
	float rangeDistance;
	float minHeight;
	float rangeHeight;
};

struct Material
{
	float4 ambient;
	float4 diffuse;
	float4 specular; // w = SpecPower
	float4 reflect;
};

struct Frame
{
	float startX;
	float startY;
	float space;
};

float3 NormalSampleToWorldSpace(float3 normalMapSample, float3 unitNormalW, float3 tangentW)
{
	float3 normalT = 2.0f * normalMapSample - 1.0f;

		float3 N = normalize(unitNormalW);
		float3 T = normalize(tangentW - dot(tangentW, N) * N);
		float3 B = cross(N, T);

	float3x3 TBN = float3x3(T, N, B);

		float3 bumpedNormalW = mul(normalT, TBN);
		return bumpedNormalW;
}

void ComputeFog(Material mat, Fog fog, float3 centreOfPlanet,
	float3 cameraPos, float3 objectPos,
	out float4 ambient, out float4 diffuse, out float4 spec)
{
	float RRH = 1.0f / fog.rangeHeight;
	float3 CP = cameraPos - centreOfPlanet;
	float3 OP = objectPos - centreOfPlanet;
	float CH = length(CP) - fog.minHeight;
	CH = clamp(CH, 0.0f, abs(CH));
	float OH = length(OP);
	OH = clamp(OH, 0.0f, abs(OH)) - fog.minHeight;
	float D = clamp(length(CP - OP), 0, fog.minHeight + fog.rangeHeight);
	float CI = (fog.rangeHeight - CH) * RRH;
	CI = D * clamp(CI, 0.0f, 1.0f);
	float OI = (fog.rangeHeight - OH) * RRH;
	OI = D * clamp(OI, 0.0f, 1.0f);
	float I = clamp(((CI + OI) - fog.minDistance) / fog.rangeDistance, 0.0f, 1.0f);
	float RI = 1.0f - I;
	ambient = RI * mat.ambient + I * fog.color;
	diffuse = RI * mat.diffuse + I * fog.color;
	spec = RI * mat.specular + I * fog.color;
}

float ComputeAtmosphereDome(Fog fog, float3 cameraPos, float3 objectPos)
{
	return clamp((length(cameraPos - objectPos) - fog.minDistance) / fog.rangeDistance, 0.0f, 1.0f);
}

void ComputeAmbientLight(Material mat, AmbientLight L,
	out float4 ambient)
{
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);

	ambient = L.power * mat.ambient * L.ambient;
}

void ComputeDirectionalLight(Material mat, DirectionalLight L,
	float3 normal, float3  toEye,
	out float4 ambient,
	out float4 diffuse,
	out float4 spec)
{
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	float3 lightVec = -L.direction;

	ambient = L.power * mat.ambient * L.ambient;

	float diffuseFactor = dot(lightVec, normal);

	diffuseFactor = diffuseFactor * diffuseFactor;

	[flatten]
	if (diffuseFactor > 0.0f)
	{
		float3 v = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(v, toEye), 0.0f), mat.specular.w);

		diffuse = L.power * diffuseFactor * mat.diffuse * L.diffuse;
		spec = L.power * specFactor * mat.specular * L.specular;
	}
}

void ComputePointLight(Material mat, PointLight L, float3 pos, float3 normal, float3 toEye,
	out float4 ambient, out float4 diffuse, out float4 spec)
{
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	float3 lightVec = L.position - pos;

		float d = length(lightVec);

	if (d > L.range)
		return;

	lightVec /= d;

	ambient = L.power * mat.ambient * L.ambient;

	float diffuseFactor = dot(lightVec, normal);

	[flatten]
	if (diffuseFactor > 0.0f)
	{
		float3 v = reflect(-lightVec, normal);
			float specFactor = pow(max(dot(v, toEye), 0.0f), mat.specular.w);

		diffuse = diffuseFactor * mat.diffuse * L.diffuse;
		spec = specFactor * mat.specular * L.specular;
	}

	float att = 1.0f / dot(L.att, float3(1.0f, d, d*d));

	diffuse *= L.power * att;
	spec *= L.power * att;
}

void ComputeSpotLight(Material mat, SpotLight L, float3 pos, float3 normal, float3 toEye,
	out float4 ambient, out float4 diffuse, out float4 spec)
{
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	float3 lightVec = -L.position;

		float d = length(lightVec);

	if (d > L.range)
		return;

	lightVec /= d;

	ambient = mat.ambient * L.ambient;


	float diffuseFactor = dot(lightVec, normal);

	[flatten]
	if (diffuseFactor > 0.0f)
	{
		float3 v = reflect(-lightVec, normal);
			float specFactor = pow(max(dot(v, toEye), 0.0f), mat.specular.w);

		diffuse = diffuseFactor * mat.diffuse * L.diffuse;
		spec = specFactor * mat.specular * L.specular;
	}

	float spot = pow(max(dot(-lightVec, L.direction), 0.0f), L.spot);

	float att = spot / dot(L.att, float3(1.0f, d, d*d));

	ambient *= L.power * spot;
	diffuse *= L.power * att;
	spec *= L.power * att;
}

float4 SkyLight(float3 normal, float3 sunVector, float4 skyColor, float light, float org)
{
	float3 bottom = float3(0.0f, -1.0f, 0.0f);

	float B = max((dot(normal, bottom) + 0.4f) * 0.714f, 0.0f);
	float S = dot(normal, sunVector);
	float SUN = asin(saturate(max(6.0f*S, 5.0f) - 5.0f))*0.6f;
	float4 C = ((1.0f - B)*skyColor + B*org)*min(S + 1.5f, 1.0f)*light;
	C.x = min(C.x + SUN, 1.0f);
	C.y = min(C.y + SUN*0.9f, 1.0f);
	C.z = min(C.z + SUN*0.5f, 1.0f);

	return C;
}


