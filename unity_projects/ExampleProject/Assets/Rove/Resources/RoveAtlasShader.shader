// Upgrade NOTE: replaced 'mul(UNITY_MATRIX_MVP,*)' with 'UnityObjectToClipPos(*)'

Shader "RoveAtlasShader" {
	Properties { target("Texture", 2D) = "white" {} }
	SubShader {
    Cull Off ZWrite Off ZTest Always Fog {Mode Off }
		Pass {
	    CGPROGRAM
        #pragma vertex vert
        #pragma fragment frag
        #include "UnityCG.cginc"

        struct V2F { 
          float4 vertex : POSITION; 
          float2 uv : TEXCOORD0;
        };

        sampler2D target;
        
        V2F vert(V2F v) 
        {
          V2F o;
          o.vertex = UnityObjectToClipPos(v.vertex);
          o.uv = MultiplyUV(UNITY_MATRIX_TEXTURE0, v.uv.xy);
          return o;
        }
        
        float4 frag(V2F v) : COLOR
        {
          return tex2D(target, v.uv);
        }
  	  ENDCG
		}
	}
}
