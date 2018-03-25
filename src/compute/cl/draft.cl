typedef struct TreeTriangle
{
  float vertex[3];
  float edges[2][3];
  float vertex_normals[3][3];
  float vertex_tangents[3][4];
  float vertex_uvs[3][2];
  float surface_normal[3];
  float inverse_area;
  uint material_id;
  uint pad;
} TreeTriangle;
///
#define TRI_VERTEX   (tri_pixel)
#define TRI_EDGES_0  (tri_pixel + 3)
#define TRI_EDGES_1  (tri_pixel + 6)
#define TRI_VN_0     (tri_pixel + 9)
#define TRI_VN_1     (tri_pixel + 12)
#define TRI_VN_2     (tri_pixel + 15)
#define TRI_VT_0     (tri_pixel + 18)
#define TRI_VT_1     (tri_pixel + 22)
#define TRI_VT_2     (tri_pixel + 26)
#define TRI_UV_0     (tri_pixel + 30)
#define TRI_UV_1     (tri_pixel + 32)
#define TRI_UV_2     (tri_pixel + 34)
#define TRI_SURF_N   (tri_pixel + 36)
#define TRI_INV_AREA (tri_pixel + 39)
#define TRI_MAT_ID   (tri_pixel + 40)
///
/*
typedef struct FourNode
{
  uint child[4];
  uint extra[2];
  float ll_bounds[6];
  float lr_bounds[6];
  float rl_bounds[6];
  float rr_bounds[6];
  float padding[2];
} FourNode;
///
#define NODE_LL        (node_pixel + 0)
#define NODE_LR        (node_pixel + 1)
#define NODE_RL        (node_pixel + 2)
#define NODE_RR        (node_pixel + 3)
#define NODE_EXTRA_0   (node_pixel + 4)
#define NODE_EXTRA_1   (node_pixel + 5)
#define NODE_LL_BOUNDS (node_pixel + 6)
#define NODE_LR_BOUNDS (node_pixel + 12)
#define NODE_RL_BOUNDS (node_pixel + 18)
#define NODE_RR_BOUNDS (node_pixel + 24)
*/
///
typedef struct TwoNode
{
  uint child[2];
  uint extra[2];
  float l_bounds[6];
  float r_bounds[6];
} TwoNode;
///
#define NODE_L        (node_pixel + 0)
#define NODE_R        (node_pixel + 1)
#define NODE_EXTRA_0   (node_pixel + 2)
#define NODE_EXTRA_1   (node_pixel + 3)
#define NODE_L_BOUNDS (node_pixel + 4)
#define NODE_R_BOUNDS (node_pixel + 10)
///
typedef struct Camera
{
  float aperture_size;
  float focal_depth;
  float transform[16];
  float root_pixel[2];
  float pixel_offset[2];
  float image_plane_distance;
  float padding;
} Camera;
///
typedef struct Material 
{
  uint map_flags;
  int double_sided;
  float albedo[4];
  float metallic;
  float smoothness;
  float glass;
  float emission;
  float albedo_bounds[4];
  float normal_bounds[4];
  float metallic_bounds[4];
  float emission_bounds[4];
} Material;
///
typedef struct Sun 
{
  float direction[4];
  float sun_intensity;
  float sky_intensity;
  float color[4];
} Sun;
///
typedef struct EnvironmentProperties
{
  float intensity;
  float rotation;
} EnvironmentProperties;
///
typedef struct EmitterIndex 
{
  uint mesh_triangle_offset;
  uint triangle_offset;
} EmitterIndex;
///
typedef struct PortalIndex 
{
  uint mesh_triangle_offset;
  uint triangle_offset;
} PortalIndex;
///
typedef struct ImageProperties {
  int tonemap;
  float gamma;
  float exposure;
  float contrast;
  float saturation;
  float brightness;
} ImageProperties;
///
#define TREE_LEAF_TRIANGLE_FACTOR 3
///
#define HM_TEST_NODES 0x0
#define HM_LEFT       0x2
#define HM_RIGHT      0x1
#define HM_BOTH       0x3
///
#define LB_NEW_SUB_TREE 0xFFFFFFFF
///
#define TREE_LEAF            0x80000000
#define TREE_LEAF_MASK       0x7FFFFFFF
#define TREE_SUB_TREE        0x40000000
#define TREE_SUB_TREE_MASK   0x3FFFFFFF
#define TREE_TRAIL_END       0x7FFFFFFF
#define TREE_NO_LEAF_HIT     0xFFFFFFFF
#define NO_TRI_HIT           INT_MAX
///
#define DOUBLE_SIDED_FLAG 0x80000000
#define DOUBLE_SIDED_MASK 0x7FFFFFFF
///
#define THROUGHPUT_THRESHOLD 0.05f
///
#define DIRAC_PDF FLT_MAX
///
#define MAP_FLAG_ALBEDO   0x1
#define MAP_FLAG_NORMAL   0x2
#define MAP_FLAG_METALLIC 0x4
#define MAP_FLAG_EMISSION 0x8
///
#define SUN_COS_THETA 0.9996f
///
#define STATE_ENVIRONMENT 0x0
#define STATE_EMISSIVE 0x1
#define STATE_MATERIAL 0x2
#define STATE_SECONDARY 0x3
#define STATE_END 0x4
///
#define RS_STATE         (ray_count * 0 + global_id)
#define RS_ORIGIN_X      (ray_count * 1 + global_id)
#define RS_ORIGIN_Y      (ray_count * 2 + global_id)
#define RS_ORIGIN_Z      (ray_count * 3 + global_id)
#define RS_DIR_X         (ray_count * 4 + global_id)
#define RS_DIR_Y         (ray_count * 5 + global_id)
#define RS_DIR_Z         (ray_count * 6 + global_id)
#define RS_U             (ray_count * 7 + global_id)
#define RS_V             (ray_count * 8 + global_id)
#define RS_HIT_DISTANCE  (ray_count * 9 + global_id)
#define RS_TRI_INDEX     (ray_count * 10 + global_id)
#define RS_LAST_BRDF_PDF (ray_count * 11 + global_id)
#define RS_THROUGHPUT_R  (ray_count * 12 + global_id)
#define RS_THROUGHPUT_G  (ray_count * 13 + global_id)
#define RS_THROUGHPUT_B  (ray_count * 14 + global_id)
#define RS_COLOR_R       (ray_count * 15 + global_id)
#define RS_COLOR_G       (ray_count * 16 + global_id)
#define RS_COLOR_B       (ray_count * 17 + global_id)
#define RS_SUN_D_X       (ray_count * 18 + global_id)
#define RS_SUN_D_Y       (ray_count * 19 + global_id)
#define RS_SUN_D_Z       (ray_count * 20 + global_id)
#define RS_SUN_R         (ray_count * 21 + global_id)
#define RS_SUN_G         (ray_count * 22 + global_id)
#define RS_SUN_B         (ray_count * 23 + global_id)
#define RS_EMITTER_D_X     (ray_count * 24 + global_id)
#define RS_EMITTER_D_Y     (ray_count * 25 + global_id)
#define RS_EMITTER_D_Z     (ray_count * 26 + global_id)
#define RS_EMITTER_T       (ray_count * 27 + global_id)
#define RS_EMITTER_R       (ray_count * 28 + global_id)
#define RS_EMITTER_G       (ray_count * 29 + global_id)
#define RS_EMITTER_B       (ray_count * 30 + global_id)
#define RS_PORTAL_D_X     (ray_count * 31 + global_id)
#define RS_PORTAL_D_Y     (ray_count * 32 + global_id)
#define RS_PORTAL_D_Z     (ray_count * 33 + global_id)
#define RS_PORTAL_R       (ray_count * 34 + global_id)
#define RS_PORTAL_G       (ray_count * 35 + global_id)
#define RS_PORTAL_B       (ray_count * 36 + global_id)
///
#define HI_EPSILON 0.01f
#define EPSILON 0.0001f
#define LO_EPSILON 0.000001f
#define PI 3.14159265359f
#define INV_PI 0.31830988618f
#define TAU 6.28318530718f
#define INV_TAU 0.15915494309
///
#define PRNG(P, SEED, R) \
{ \
  uint hash = SEED; \
  hash ^= P; \
  hash += (hash << 10); \
  hash ^= (hash >>  6); \
  hash += (hash <<  3); \
  hash ^= (hash >> 11); \
  hash += (hash << 15); \
  hash ^= hash >> 16; \
  hash *= 0x85EBCA6B; \
  hash ^= hash >> 13; \
  hash *= 0xC2B2AE35; \
  hash ^= hash >> 16; \
  hash &= 0x007FFFFFu; \
  hash |= 0x3F800000u; \
  R = *((float*)&hash) - 1.0f; \
	R = fmax(EPSILON, R); \
}
///
#define TRANSFORM_X(P, MAT, R) { \
  R[0] = P[0] * MAT[0] + P[1] * MAT[4] + P[2] * MAT[8] + MAT[12]; \
}
#define TRANSFORM_Y(P, MAT, R) { \
  R[1] = P[0] * MAT[1] + P[1] * MAT[5] + P[2] * MAT[9] + MAT[13]; \
}
#define TRANSFORM_Z(P, MAT, R) { \
  R[2] = P[0] * MAT[2] + P[1] * MAT[6] + P[2] * MAT[10] + MAT[14]; \
}
#define TRANSFORM_W(P, MAT, R) { \
  R[3] = P[0] * MAT[3] + P[1] * MAT[7] + P[2] * MAT[11] + MAT[15]; \
}
///
#define DOT(A, B) (A[0] * B[0] + A[1] * B[1] + A[2] * B[2])
///
#define CROSS(A, B, C) { \
  C[0] = A[1] * B[2] - A[2] * B[1]; \
  C[1] = A[2] * B[0] - A[0] * B[2]; \
  C[2] = A[0] * B[1] - A[1] * B[0]; \
}
///
#define NOT_ZERO(A) copysign(fmax(LO_EPSILON, fabs(A)), (A))
///
#define NORMALIZE(V) \
{ \
  float length = native_sqrt(V[0] * V[0] + V[1] * V[1] + V[2] * V[2]); \
  float inv_length = 1.0f / NOT_ZERO(length); \
  V[0] *= inv_length; \
  V[1] *= inv_length; \
  V[2] *= inv_length; \
}
///
#define GGX_SAMPLE(D_I, ROUGHNESS, RXF, RYF, N) \
{ \
  float stretched[3]; \
  stretched[0] = ROUGHNESS[0] * D_I[0]; \
  stretched[1] = ROUGHNESS[1] * D_I[1]; \
  stretched[2] = D_I[2]; \
  NORMALIZE(stretched); \
  float up[3]; \
  up[0] = 0.0f; \
  up[1] = 0.0f; \
  up[2] = 1.0f; \
  float t1[3]; \
  CROSS(stretched, up, t1); \
  if (stretched[2] < 0.9999f) { \
    NORMALIZE(t1); \
  } else { \
    t1[0] = 1.0f; \
    t1[1] = 0.0f; \
    t1[2] = 0.0f; \
  } \
  float t2[3]; \
  CROSS(t1, stretched, t2); \
  float a = 1.0f / (1.0f + stretched[2]); \
  float r = native_sqrt(RXF); \
  float phi = (RYF < a) ? RYF / a * PI : PI + (RYF - a) / (1.0f - a) * PI; \
  float p1 = r * native_cos(phi); \
  float p2 = r * native_sin(phi) * ((RYF < a) ? 1.0f : stretched[2]); \
  N[0] = ROUGHNESS[0] * \
         (p1 * t1[0] + p2 * t2[0] + native_sqrt(1.0f - p1 * p1 - p2 * p2) * stretched[0]); \
  N[1] = ROUGHNESS[1] * \
         (p1 * t1[1] + p2 * t2[1] + native_sqrt(1.0f - p1 * p1 - p2 * p2) * stretched[1]); \
  N[2] = (p1 * t1[2] + p2 * t2[2] + native_sqrt(1.0f - p1 * p1 - p2 * p2) * stretched[2]); \
  NORMALIZE(N); \
}
///
#define TAN_THETA(V, T) \
{ \
  float temp = 1.0f - V[2] * V[2]; \
  if (temp <= 0.0f) { \
    T = 0.0f; \
  } else { \
    T = native_sqrt(temp) / V[2]; \
  } \
}
///
#define SIN_THETA_2(V, T) \
{ \
  T = 1.0f - V[2] * V[2]; \
}
///
#define PROJECT_ROUGHNESS(V, ROUGHNESS, R) \
{ \
  float sin_theta_2; \
  SIN_THETA_2(V, sin_theta_2); \
  float inv_sin_theta_2 = 1.0f / sin_theta_2; \
  if ((ROUGHNESS[0] == ROUGHNESS[1]) || (inv_sin_theta_2 <= 0.0f)) { \
    R = ROUGHNESS[0]; \
  } else { \
    float cos_phi_2 = V[0] * V[0] * inv_sin_theta_2; \
    float sin_phi_2 = V[1] * V[1] * inv_sin_theta_2; \
    R = native_sqrt(cos_phi_2 * ROUGHNESS[0] * ROUGHNESS[0] + \
             sin_phi_2 * ROUGHNESS[1] * ROUGHNESS[1]); \
  } \
}
///
#define HYPOT_2(A, B, R) \
{ \
  if (fabs(A) > fabs(B)) { \
    R = B / A; \
    R = fabs(A) * native_sqrt(1.0f + R * R); \
  } else if (B != 0.0f) { \
    R = A / B; \
    R = fabs(B) * native_sqrt(1.0f + R * R); \
  } else {\
    R = 0.0f; \
  } \
}
///
#define SMITH_G1(V, N, ROUGHNESS, G1) \
{ \
  if ((DOT(N, V) * V[2]) <= 0.0f) { \
    G1 = 0.0f; \
  } else { \
    float tan_theta; \
    TAN_THETA(V, tan_theta); \
    tan_theta = fabs(tan_theta); \
    if (tan_theta == 0.0f) { \
      G1 = 1.0f; \
    } else { \
      float alpha; \
      PROJECT_ROUGHNESS(V, ROUGHNESS, alpha); \
      float root = alpha * tan_theta; \
      float hypot_2; \
      HYPOT_2(1.0f, root, hypot_2); \
      G1 = 2.0f / (1.0f + hypot_2); \
    } \
  } \
}
///
#define GGX_EVAL(N, ROUGHNESS, R) \
{ \
  if (N[2] <= 0.0f) { \
    R = 0.0f; \
  } else { \
    float cos_theta_2 = N[2] * N[2]; \
    float exponent = ((N[0] * N[0]) / (ROUGHNESS[0] * ROUGHNESS[0]) + \
                      (N[1] * N[1]) / (ROUGHNESS[1] * ROUGHNESS[1])) / cos_theta_2; \
    float root = (1.0f + exponent) * cos_theta_2; \
    R = 1.0f / (PI * ROUGHNESS[0] * ROUGHNESS[1] * root * root); \
  } \
}
///
#define AVG_TONEMAP(RGB) \
{ \
  RGB[0] *= inv_sample_count; \
  RGB[1] *= inv_sample_count; \
  RGB[2] *= inv_sample_count; \
  RGB[0] *= image_properties.exposure + 2.0f; \
  RGB[1] *= image_properties.exposure + 2.0f; \
  RGB[2] *= image_properties.exposure + 2.0f; \
  if (image_properties.tonemap) { \
    RGB[0] = ((RGB[0] * (0.15f * RGB[0] + 0.10f * 0.5f) + 0.20f * 0.02f) / \
              (RGB[0] * (0.15f * RGB[0] + 0.5f) + 0.2f * 0.3f)) - (0.02f / 0.3f); \
    RGB[1] = ((RGB[1] * (0.15f * RGB[1] + 0.10f * 0.5f) + 0.20f * 0.02f) / \
              (RGB[1] * (0.15f * RGB[1] + 0.5f) + 0.2f * 0.3f)) - (0.02f / 0.3f); \
    RGB[2] = ((RGB[2] * (0.15f * RGB[2] + 0.10f * 0.5f) + 0.20f * 0.02f) / \
              (RGB[2] * (0.15f * RGB[2] + 0.5f) + 0.2f * 0.3f)) - (0.02f / 0.3f); \
    float whitescale = 11.2f; \
    whitescale = ((whitescale * (0.15f * whitescale + 0.10f * 0.5f) + \
                  0.20f * 0.02f) / (whitescale * (0.15f * whitescale + 0.5f) + \
                  0.2f * 0.3f)) - (0.02f / 0.3f); \
    whitescale = 1.0f / whitescale; \
    RGB[0] *= whitescale; \
    RGB[1] *= whitescale; \
    RGB[2] *= whitescale; \
  } \
  RGB[0] = pow(RGB[0], 1.0f / image_properties.gamma); \
  RGB[1] = pow(RGB[1], 1.0f / image_properties.gamma); \
  RGB[2] = pow(RGB[2], 1.0f / image_properties.gamma); \
}
///
#define MIS(A, B) ((A * A) / (A * A + B * B))
///
#define CONCENTRIC_HEMI(R0, R1, D) \
{ \
  R0 = 2.0f * R0 - 1.0f; \
  R1 = 2.0f * R1 - 1.0f; \
  float phi, r; \
  if (R0 == 0.0f && R1 == 0.0f) { \
    r = phi = 0.0f; \
  } else if ((R0 * R0) > (R1 * R1)) { \
    r = R0; \
    phi = (PI * 0.25f) * (R1 / (R0)); \
  } else { \
    r = R1; \
    phi = (PI * 0.5f) - (R0 / (R1)) * (PI * 0.25f); \
  } \
  float cos_phi = native_cos(phi); \
  float sin_phi = native_sin(phi); \
  D[0] = r * cos_phi; \
  D[1] = r * sin_phi; \
  D[2] = native_sqrt(fmax(0.0f, 1.0f - D[0] * D[0] - D[1] * D[1])); \
  NORMALIZE(D); \
}
///
#define GET_BASIS(N, T, B) \
{\
  float z = 1.0f + N[2]; \
  float a = 1.0f / NOT_ZERO(z); \
  float b = -N[0] * N[1] * a; \
  T[0] = 1.0f - N[0] * N[0] * a; \
  T[1] = b; \
  T[2] = -N[0]; \
  B[0] = b; \
  B[1] = 1.0f - N[1] * N[1] * a; \
  B[2] = -N[1]; \
}
///
#define GET_VT_BASIS(N, T, B) \
{\
  float flip_binormal; \
  if (use_triangles_image) { \
    T[0] = w * read_imagef(triangles_image, TRI_VT_0).x + \
           u * read_imagef(triangles_image, TRI_VT_1).x + \
           v * read_imagef(triangles_image, TRI_VT_2).x; \
    T[1] = w * read_imagef(triangles_image, TRI_VT_0 + 1).x + \
           u * read_imagef(triangles_image, TRI_VT_1 + 1).x + \
           v * read_imagef(triangles_image, TRI_VT_2 + 1).x; \
    T[2] = w * read_imagef(triangles_image, TRI_VT_0 + 2).x + \
           u * read_imagef(triangles_image, TRI_VT_1 + 2).x + \
           v * read_imagef(triangles_image, TRI_VT_2 + 2).x; \
    flip_binormal = w * read_imagef(triangles_image, TRI_VT_0 + 3).x + \
                    u * read_imagef(triangles_image, TRI_VT_1 + 3).x + \
                    v * read_imagef(triangles_image, TRI_VT_2 + 3).x; \
  } else { \
    T[0] = w * triangles[tri_index].vertex_tangents[0][0] + \
           u * triangles[tri_index].vertex_tangents[1][0] + \
           v * triangles[tri_index].vertex_tangents[2][0]; \
    T[1] = w * triangles[tri_index].vertex_tangents[0][1] + \
           u * triangles[tri_index].vertex_tangents[1][1] + \
           v * triangles[tri_index].vertex_tangents[2][1]; \
    T[2] = w * triangles[tri_index].vertex_tangents[0][2] + \
           u * triangles[tri_index].vertex_tangents[1][2] + \
           v * triangles[tri_index].vertex_tangents[2][2]; \
    flip_binormal = w * triangles[tri_index].vertex_tangents[0][3] + \
                    u * triangles[tri_index].vertex_tangents[1][3] + \
                    v * triangles[tri_index].vertex_tangents[2][3]; \
  } \
  CROSS(N, T, B); \
  if (flip_binormal < 0.0f) { \
    B[0] = -B[0]; \
    B[1] = -B[1]; \
    B[2] = -B[2]; \
  } \
}
///
#define RELATIVE_TO_BASIS(V, N, T, B, D) \
{\
  D[0] = DOT(V, T); \
  D[1] = DOT(V, B); \
  D[2] = DOT(V, N); \
}
///
#define ROTATE_TO_BASIS(V, N, T, B, D) \
{\
  D[0] = V[0] * T[0] + V[1] * B[0] + V[2] * N[0]; \
  D[1] = V[0] * T[1] + V[1] * B[1] + V[2] * N[1]; \
  D[2] = V[0] * T[2] + V[1] * B[2] + V[2] * N[2]; \
}
///
#define CONDUCTOR_FRESNEL(I, IOR, K, F) \
{ \
  float cos_theta_2 = I * I; \
  float temp = (IOR * IOR + K * K) * cos_theta_2; \
  float rp2 = (temp - (IOR * (2.0f * I)) + 1.0f) / \
              (temp + (IOR * (2.0f * I)) + 1.0f); \
  float tempf = IOR * IOR + K * K; \
  float rs2 = (tempf - (IOR * (2.0f * I)) + cos_theta_2) / \
              (tempf + (IOR * (2.0f * I)) + cos_theta_2); \
  F = 0.5f * (rp2 + rs2); \
}
///
#define DIELECTRIC_FRESNEL(I, IOR, COS_THETA_T, F) \
{ \
  float scale = (I > 0.0f) ? 1.0f / IOR : IOR; \
  float cos_theta_t2 = 1.0f - (1.0f - I * I) * (scale * scale); \
  if (cos_theta_t2 < 0.0f) { \
    COS_THETA_T = 0.0f; \
    F = 1.0f; \
  } else { \
    float cos_theta_i = fabs(I); \
    float _cos_theta_t = native_sqrt(cos_theta_t2); \
    float rs = (cos_theta_i - IOR * _cos_theta_t) / \
               (cos_theta_i + IOR * _cos_theta_t); \
    float rp = (IOR * cos_theta_i - _cos_theta_t) / \
               (IOR * cos_theta_i + _cos_theta_t); \
    COS_THETA_T = (I > 0.0f) ? -_cos_theta_t : _cos_theta_t; \
    F = 0.5f * (rs * rs + rp * rp); \
  } \
}
///
#define TRACE(USE_NODES_IMAGE, USE_TRIANGLES_IMAGE, PRIMARY_RAY) \
{ \
  float id[3], ood[3]; \
  id[0] = 1.0f / NOT_ZERO(d[0]); \
  id[1] = 1.0f / NOT_ZERO(d[1]); \
  id[2] = 1.0f / NOT_ZERO(d[2]); \
  ood[0] = o[0] * id[0]; \
  ood[1] = o[1] * id[1]; \
  ood[2] = o[2] * id[2]; \
  int h_tri_index = NO_TRI_HIT; \
  int h_material_type; \
  float h_u, h_v; \
  float h_t = FLT_MAX; \
  uint top_tree_height; \
  if (USE_NODES_IMAGE) { \
    int node_pixel = (top_tree_offset << 4); \
    top_tree_height = read_imageui(nodes_image, NODE_EXTRA_0).x; \
  } else { \
    top_tree_height = nodes[top_tree_offset].extra[0]; \
  } \
  uint tree_height = top_tree_height; \
  uint tree_offset = top_tree_offset; \
  uint node_offset = 0; \
  uint trail = 0x0; \
  uint key = 0x0; \
  uint level_bit = 0x1 << (top_tree_height - 1); \
  uint hit_leaf = TREE_NO_LEAF_HIT; \
  uint mesh_triangle_offset = 0; \
  uint hit_mask = HM_TEST_NODES; \
  while (true) { \
    while (hit_leaf >= TREE_NO_LEAF_HIT) { \
      uint node_index = node_offset + tree_offset; \
      int node_pixel = (node_index << 4); \
      if (level_bit == LB_NEW_SUB_TREE) { \
        if (USE_NODES_IMAGE) { \
          tree_height = read_imageui(nodes_image, NODE_EXTRA_0).x; \
          mesh_triangle_offset = read_imageui(nodes_image, NODE_EXTRA_1).x; \
        } else { \
          tree_height = nodes[node_index].extra[0]; \
          mesh_triangle_offset = nodes[node_index].extra[1]; \
        } \
        trail <<= tree_height; \
        key <<= tree_height; \
        level_bit = 0x1 << (tree_height - 1); \
      } \
      if (hit_mask == HM_TEST_NODES) { \
        float l_bounds[6], r_bounds[6]; \
        if (USE_NODES_IMAGE) { \
          l_bounds[0] = read_imagef(nodes_image, NODE_L_BOUNDS).x; \
          l_bounds[1] = read_imagef(nodes_image, NODE_L_BOUNDS + 1).x; \
          l_bounds[2] = read_imagef(nodes_image, NODE_L_BOUNDS + 2).x; \
          l_bounds[3] = read_imagef(nodes_image, NODE_L_BOUNDS + 3).x; \
          l_bounds[4] = read_imagef(nodes_image, NODE_L_BOUNDS + 4).x; \
          l_bounds[5] = read_imagef(nodes_image, NODE_L_BOUNDS + 5).x; \
          r_bounds[0] = read_imagef(nodes_image, NODE_R_BOUNDS).x; \
          r_bounds[1] = read_imagef(nodes_image, NODE_R_BOUNDS + 1).x; \
          r_bounds[2] = read_imagef(nodes_image, NODE_R_BOUNDS + 2).x; \
          r_bounds[3] = read_imagef(nodes_image, NODE_R_BOUNDS + 3).x; \
          r_bounds[4] = read_imagef(nodes_image, NODE_R_BOUNDS + 4).x; \
          r_bounds[5] = read_imagef(nodes_image, NODE_R_BOUNDS + 5).x; \
        } else { \
          l_bounds[0] = nodes[node_index].l_bounds[0]; \
          l_bounds[1] = nodes[node_index].l_bounds[1]; \
          l_bounds[2] = nodes[node_index].l_bounds[2]; \
          l_bounds[3] = nodes[node_index].l_bounds[3]; \
          l_bounds[4] = nodes[node_index].l_bounds[4]; \
          l_bounds[5] = nodes[node_index].l_bounds[5]; \
          r_bounds[0] = nodes[node_index].r_bounds[0]; \
          r_bounds[1] = nodes[node_index].r_bounds[1]; \
          r_bounds[2] = nodes[node_index].r_bounds[2]; \
          r_bounds[3] = nodes[node_index].r_bounds[3]; \
          r_bounds[4] = nodes[node_index].r_bounds[4]; \
          r_bounds[5] = nodes[node_index].r_bounds[5]; \
        } \
        float l_lo[3], l_hi[3]; \
        l_lo[0] = l_bounds[0] * id[0] - ood[0]; \
        l_lo[1] = l_bounds[1] * id[1] - ood[1]; \
        l_lo[2] = l_bounds[2] * id[2] - ood[2]; \
        l_hi[0] = l_bounds[3] * id[0] - ood[0]; \
        l_hi[1] = l_bounds[4] * id[1] - ood[1]; \
        l_hi[2] = l_bounds[5] * id[2] - ood[2]; \
        float r_lo[3], r_hi[3]; \
        r_lo[0] = r_bounds[0] * id[0] - ood[0]; \
        r_lo[1] = r_bounds[1] * id[1] - ood[1]; \
        r_lo[2] = r_bounds[2] * id[2] - ood[2]; \
        r_hi[0] = r_bounds[3] * id[0] - ood[0]; \
        r_hi[1] = r_bounds[4] * id[1] - ood[1]; \
        r_hi[2] = r_bounds[5] * id[2] - ood[2]; \
        float l_near = fmax(fmax(fmax(fmin(l_lo[0], l_hi[0]),\
                                      fmin(l_lo[1], l_hi[1])),\
                                      fmin(l_lo[2], l_hi[2])), 0.0f); \
        float l_far = fmin(fmin(fmax(l_lo[0], l_hi[0]),\
                                fmax(l_lo[1], l_hi[1])),\
                                fmax(l_lo[2], l_hi[2])); \
        float r_near = fmax(fmax(fmax(fmin(r_lo[0], r_hi[0]),\
                                      fmin(r_lo[1], r_hi[1])),\
                                      fmin(r_lo[2], r_hi[2])), 0.0f); \
        float r_far = fmin(fmin(fmax(r_lo[0], r_hi[0]),\
                                fmax(r_lo[1], r_hi[1])),\
                                fmax(r_lo[2], r_hi[2])); \
        hit_mask = (((l_far >= l_near) && (l_near < h_t)) << 1) | \
                    ((r_far >= r_near) && (r_near < h_t)); \
        if (hit_mask == HM_BOTH) { \
          hit_mask = (l_near < r_near) + 1; \
          trail |= level_bit; \
        } \
      } \
      if (hit_mask == HM_TEST_NODES) { \
        break; \
      } \
      if (hit_mask == HM_RIGHT) { \
        key |= level_bit; \
        node_offset += level_bit; \
      } else { \
        ++node_offset; \
      } \
      int child = (hit_mask & 0x1); \
      if (USE_NODES_IMAGE) { \
        child = read_imageui(nodes_image, NODE_L + child).x; \
      } else { \
        child = nodes[node_index].child[child]; \
      } \
      hit_mask = HM_TEST_NODES; \
      level_bit >>= 1; \
      if (child & TREE_LEAF) { \
        hit_leaf = (child & TREE_LEAF_MASK); \
        break; \
      } else if (child & TREE_SUB_TREE) { \
        tree_offset = (child & TREE_SUB_TREE_MASK); \
        level_bit = LB_NEW_SUB_TREE; \
        node_offset = 0; \
      } \
    } \
    if (hit_leaf != TREE_NO_LEAF_HIT) { \
      uint tri_count = (hit_leaf >> (31 - TREE_LEAF_TRIANGLE_FACTOR)); \
      uint tri_offset = mesh_triangle_offset + \
                        (hit_leaf & \
                        (TREE_LEAF_MASK >> TREE_LEAF_TRIANGLE_FACTOR)); \
      for (uint ti = tri_offset; ti <= tri_offset + tri_count; ++ti) { \
        int tri_pixel = ti * 42; \
        float vertex[3]; \
        float edges[2][3]; \
        float surface_normal[3]; \
        if (USE_TRIANGLES_IMAGE) { \
          vertex[0] = read_imagef(triangles_image, TRI_VERTEX).x; \
          vertex[1] = read_imagef(triangles_image, TRI_VERTEX + 1).x; \
          vertex[2] = read_imagef(triangles_image, TRI_VERTEX + 2).x; \
          edges[0][0] = read_imagef(triangles_image, TRI_EDGES_0).x; \
          edges[0][1] = read_imagef(triangles_image, TRI_EDGES_0 + 1).x; \
          edges[0][2] = read_imagef(triangles_image, TRI_EDGES_0 + 2).x; \
          edges[1][0] = read_imagef(triangles_image, TRI_EDGES_1).x; \
          edges[1][1] = read_imagef(triangles_image, TRI_EDGES_1 + 1).x; \
          edges[1][2] = read_imagef(triangles_image, TRI_EDGES_1 + 2).x; \
          surface_normal[0] = read_imagef(triangles_image, TRI_SURF_N).x; \
          surface_normal[1] = read_imagef(triangles_image, TRI_SURF_N + 1).x; \
          surface_normal[2] = read_imagef(triangles_image, TRI_SURF_N + 2).x; \
        } else { \
          vertex[0] = triangles[ti].vertex[0]; \
          vertex[1] = triangles[ti].vertex[1]; \
          vertex[2] = triangles[ti].vertex[2]; \
          edges[0][0] = triangles[ti].edges[0][0]; \
          edges[0][1] = triangles[ti].edges[0][1]; \
          edges[0][2] = triangles[ti].edges[0][2]; \
          edges[1][0] = triangles[ti].edges[1][0]; \
          edges[1][1] = triangles[ti].edges[1][1]; \
          edges[1][2] = triangles[ti].edges[1][2]; \
          surface_normal[0] = triangles[ti].surface_normal[0]; \
          surface_normal[1] = triangles[ti].surface_normal[1]; \
          surface_normal[2] = triangles[ti].surface_normal[2]; \
        } \
        float c[3], r[3]; \
        c[0] = vertex[0] - o[0]; \
        c[1] = vertex[1] - o[1]; \
        c[2] = vertex[2] - o[2]; \
        r[0] = (d[1] * c[2]) - (d[2] * c[1]); \
        r[1] = (d[2] * c[0]) - (d[0] * c[2]); \
        r[2] = (d[0] * c[1]) - (d[1] * c[0]); \
        float u, v; \
        u = r[0] * edges[1][0] + r[1] * edges[1][1] + \
            r[2] * edges[1][2]; \
        v = r[0] * edges[0][0] + r[1] * edges[0][1] + \
            r[2] * edges[0][2]; \
        float dot = DOT(-d, surface_normal); \
        float t = c[0] * surface_normal[0] + \
                  c[1] * surface_normal[1] + \
                  c[2] * surface_normal[2]; \
        float abs_dot = fabs(dot); \
        float sign_dot = copysign(1.0f, -dot); \
        float rcp_dot = 1.0f / NOT_ZERO(abs_dot); \
        u *= sign_dot; \
        v *= sign_dot; \
        t *= sign_dot * rcp_dot; \
        if ((dot != 0.0f) && (u >= 0.0f) && (v >= 0.0f) && \
            ((u + v) < abs_dot) && (t > 0.0f) && (t < h_t)) { \
          uint material_id; \
          float vertex_uvs[3][2]; \
          if (USE_TRIANGLES_IMAGE) { \
            material_id = read_imageui(triangles_image, TRI_MAT_ID).x; \
            vertex_uvs[0][0] = read_imagef(triangles_image, TRI_UV_0).x; \
            vertex_uvs[0][1] = read_imagef(triangles_image, TRI_UV_0 + 1).x; \
            vertex_uvs[1][0] = read_imagef(triangles_image, TRI_UV_1).x; \
            vertex_uvs[1][1] = read_imagef(triangles_image, TRI_UV_1 + 1).x; \
            vertex_uvs[2][0] = read_imagef(triangles_image, TRI_UV_2).x; \
            vertex_uvs[2][1] = read_imagef(triangles_image, TRI_UV_2 + 1).x; \
          } else { \
            material_id = triangles[ti].material_id; \
            vertex_uvs[0][0] = triangles[ti].vertex_uvs[0][0]; \
            vertex_uvs[0][1] = triangles[ti].vertex_uvs[0][1]; \
            vertex_uvs[1][0] = triangles[ti].vertex_uvs[1][0]; \
            vertex_uvs[1][1] = triangles[ti].vertex_uvs[1][1]; \
            vertex_uvs[2][0] = triangles[ti].vertex_uvs[2][0]; \
            vertex_uvs[2][1] = triangles[ti].vertex_uvs[2][1]; \
          } \
          u *= rcp_dot; \
          v *= rcp_dot; \
          float w = 1.0f - u - v; \
          float uv[2]; \
          uv[0] = w * vertex_uvs[0][0] + \
                  u * vertex_uvs[1][0] + \
                  v * vertex_uvs[2][0]; \
          uv[1] = w * vertex_uvs[0][1] + \
                  u * vertex_uvs[1][1] + \
                  v * vertex_uvs[2][1]; \
          if ((materials[material_id].glass > 0.0f) || \
              (materials[material_id].emission > 0.0f) || \
              (materials[material_id].double_sided) || (dot > 0.0f)) { \
            float alpha = 1.0f; \
            float alpha_sample = 0.0f; \
            float emission = materials[material_id].emission; \
            if (materials[material_id].map_flags & MAP_FLAG_EMISSION) { \
              float2 emission_uv; \
              emission_uv.x = uv[0]; \
              emission_uv.y = uv[1]; \
              emission_uv.x = \
                mix(materials[material_id].emission_bounds[0] + 0.5f, \
                     materials[material_id].emission_bounds[1] - 0.5f, \
                     emission_uv.x); \
              emission_uv.y = \
                mix(materials[material_id].emission_bounds[2] + 0.5f, \
                     materials[material_id].emission_bounds[3] - 0.5f, \
                     emission_uv.y); \
              const sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE | \
                                        CLK_ADDRESS_REPEAT | CLK_FILTER_LINEAR; \
              float4 emission_map = read_imagef(emission_atlas, sampler, emission_uv); \
              emission *= fmax(fmax(emission_map.x, emission_map.y), \
                               emission_map.z); \
            } \
            if (emission == 0.0f) { \
              alpha = materials[material_id].albedo[3]; \
              PRNG(global_id, b_seeds.w, alpha_sample); \
              if (materials[material_id].map_flags & MAP_FLAG_ALBEDO) { \
                float2 albedo_uv; \
                albedo_uv.x = uv[0]; \
                albedo_uv.y = uv[1]; \
                albedo_uv.x = \
                  mix(materials[material_id].albedo_bounds[0] + 0.5f, \
                       materials[material_id].albedo_bounds[1] - 0.5f, \
                       albedo_uv.x); \
                albedo_uv.y = \
                  mix(materials[material_id].albedo_bounds[2] + 0.5f, \
                       materials[material_id].albedo_bounds[3] - 0.5f, \
                       albedo_uv.y); \
                const sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE | \
                                          CLK_ADDRESS_REPEAT | CLK_FILTER_LINEAR; \
                float4 albedo_map = read_imagef(albedo_atlas, sampler, albedo_uv); \
                alpha *= albedo_map.w; \
              } \
            } \
            if (alpha_sample < alpha) { \
              h_t = t; \
              h_u = u; \
              h_v = v; \
              if (emission > 0.0f) { \
                h_material_type = STATE_EMISSIVE; \
              } else { \
                h_material_type = STATE_MATERIAL; \
              } \
              h_tri_index = ti; \
            } \
          } \
        } \
      } \
      hit_leaf = TREE_NO_LEAF_HIT; \
    } \
    if (trail == 0x0) { \
      break; \
    } \
    uint shift = 31 - clz(trail & -trail); \
    if (shift >= tree_height) { \
      shift -= tree_height; \
      trail >>= tree_height; \
      key >>= tree_height; \
      tree_height = top_tree_height; \
      tree_offset = top_tree_offset; \
      mesh_triangle_offset = 0; \
    } \
    level_bit = (0x1 << shift); \
    hit_mask = ((key & level_bit) > 0) + 1; \
    trail ^= level_bit; \
    key = (key & (0xFFFFFFFF << (shift + 1))); \
    uint local_key = key & (0xFFFFFFFF >> (32 - tree_height)); \
    node_offset = local_key + ((32 - popcount(local_key)) - \
                               (32 - tree_height) - (shift + 1)); \
  } \
  ray_state[RS_DIR_X] = d[0]; \
  ray_state[RS_DIR_Y] = d[1]; \
  ray_state[RS_DIR_Z] = d[2]; \
  if (h_tri_index != NO_TRI_HIT) {\
    ray_state[RS_STATE] = h_material_type; \
    o[0] += d[0] * h_t; \
    o[1] += d[1] * h_t; \
    o[2] += d[2] * h_t; \
    ray_state[RS_ORIGIN_X] = o[0]; \
    ray_state[RS_ORIGIN_Y] = o[1]; \
    ray_state[RS_ORIGIN_Z] = o[2]; \
    ray_state[RS_U] = h_u; \
    ray_state[RS_V] = h_v; \
    ray_state[RS_HIT_DISTANCE] = h_t; \
    ray_state[RS_TRI_INDEX] = h_tri_index; \
  } else {\
    ray_state[RS_STATE] = STATE_ENVIRONMENT; \
  }\
}
///
#define OCCLUSION(O, D, USE_NODES_IMAGE, USE_TRIANGLES_IMAGE) \
{ \
  float id[3], ood[3]; \
  id[0] = 1.0f / NOT_ZERO(D[0]); \
  id[1] = 1.0f / NOT_ZERO(D[1]); \
  id[2] = 1.0f / NOT_ZERO(D[2]); \
  ood[0] = O[0] * id[0]; \
  ood[1] = O[1] * id[1]; \
  ood[2] = O[2] * id[2]; \
  uint top_tree_height; \
  if (USE_NODES_IMAGE) { \
    int node_pixel = (top_tree_offset << 4); \
    top_tree_height = read_imageui(nodes_image, NODE_EXTRA_0).x; \
  } else { \
    top_tree_height = nodes[top_tree_offset].extra[0]; \
  } \
  uint tree_height = top_tree_height; \
  uint tree_offset = top_tree_offset; \
  uint node_offset = 0; \
  uint trail = 0x0; \
  uint key = 0x0; \
  uint level_bit = 0x1 << (top_tree_height - 1); \
  uint hit_leaf = TREE_NO_LEAF_HIT; \
  uint mesh_triangle_offset = 0; \
  uint hit_mask = HM_TEST_NODES; \
  while (true) { \
    while (hit_leaf >= TREE_NO_LEAF_HIT) { \
      uint node_index = node_offset + tree_offset; \
      int node_pixel = (node_index << 4); \
      if (level_bit == LB_NEW_SUB_TREE) { \
        if (USE_NODES_IMAGE) { \
          tree_height = read_imageui(nodes_image, NODE_EXTRA_0).x; \
          mesh_triangle_offset = read_imageui(nodes_image, NODE_EXTRA_1).x; \
        } else { \
          tree_height = nodes[node_index].extra[0]; \
          mesh_triangle_offset = nodes[node_index].extra[1]; \
        } \
        trail <<= tree_height; \
        key <<= tree_height; \
        level_bit = 0x1 << (tree_height - 1); \
      } \
      if (hit_mask == HM_TEST_NODES) { \
        float l_bounds[6], r_bounds[6]; \
        if (USE_NODES_IMAGE) { \
          l_bounds[0] = read_imagef(nodes_image, NODE_L_BOUNDS).x; \
          l_bounds[1] = read_imagef(nodes_image, NODE_L_BOUNDS + 1).x; \
          l_bounds[2] = read_imagef(nodes_image, NODE_L_BOUNDS + 2).x; \
          l_bounds[3] = read_imagef(nodes_image, NODE_L_BOUNDS + 3).x; \
          l_bounds[4] = read_imagef(nodes_image, NODE_L_BOUNDS + 4).x; \
          l_bounds[5] = read_imagef(nodes_image, NODE_L_BOUNDS + 5).x; \
          r_bounds[0] = read_imagef(nodes_image, NODE_R_BOUNDS).x; \
          r_bounds[1] = read_imagef(nodes_image, NODE_R_BOUNDS + 1).x; \
          r_bounds[2] = read_imagef(nodes_image, NODE_R_BOUNDS + 2).x; \
          r_bounds[3] = read_imagef(nodes_image, NODE_R_BOUNDS + 3).x; \
          r_bounds[4] = read_imagef(nodes_image, NODE_R_BOUNDS + 4).x; \
          r_bounds[5] = read_imagef(nodes_image, NODE_R_BOUNDS + 5).x; \
        } else { \
          l_bounds[0] = nodes[node_index].l_bounds[0]; \
          l_bounds[1] = nodes[node_index].l_bounds[1]; \
          l_bounds[2] = nodes[node_index].l_bounds[2]; \
          l_bounds[3] = nodes[node_index].l_bounds[3]; \
          l_bounds[4] = nodes[node_index].l_bounds[4]; \
          l_bounds[5] = nodes[node_index].l_bounds[5]; \
          r_bounds[0] = nodes[node_index].r_bounds[0]; \
          r_bounds[1] = nodes[node_index].r_bounds[1]; \
          r_bounds[2] = nodes[node_index].r_bounds[2]; \
          r_bounds[3] = nodes[node_index].r_bounds[3]; \
          r_bounds[4] = nodes[node_index].r_bounds[4]; \
          r_bounds[5] = nodes[node_index].r_bounds[5]; \
        } \
        float l_lo[3], l_hi[3]; \
        l_lo[0] = l_bounds[0] * id[0] - ood[0]; \
        l_lo[1] = l_bounds[1] * id[1] - ood[1]; \
        l_lo[2] = l_bounds[2] * id[2] - ood[2]; \
        l_hi[0] = l_bounds[3] * id[0] - ood[0]; \
        l_hi[1] = l_bounds[4] * id[1] - ood[1]; \
        l_hi[2] = l_bounds[5] * id[2] - ood[2]; \
        float r_lo[3], r_hi[3]; \
        r_lo[0] = r_bounds[0] * id[0] - ood[0]; \
        r_lo[1] = r_bounds[1] * id[1] - ood[1]; \
        r_lo[2] = r_bounds[2] * id[2] - ood[2]; \
        r_hi[0] = r_bounds[3] * id[0] - ood[0]; \
        r_hi[1] = r_bounds[4] * id[1] - ood[1]; \
        r_hi[2] = r_bounds[5] * id[2] - ood[2]; \
        float l_near = fmax(fmax(fmax(fmin(l_lo[0], l_hi[0]),\
                                      fmin(l_lo[1], l_hi[1])),\
                                      fmin(l_lo[2], l_hi[2])), 0.0f); \
        float l_far = fmin(fmin(fmax(l_lo[0], l_hi[0]),\
                                fmax(l_lo[1], l_hi[1])),\
                                fmax(l_lo[2], l_hi[2])); \
        float r_near = fmax(fmax(fmax(fmin(r_lo[0], r_hi[0]),\
                                      fmin(r_lo[1], r_hi[1])),\
                                      fmin(r_lo[2], r_hi[2])), 0.0f); \
        float r_far = fmin(fmin(fmax(r_lo[0], r_hi[0]),\
                                fmax(r_lo[1], r_hi[1])),\
                                fmax(r_lo[2], r_hi[2])); \
        hit_mask = (((l_far >= l_near) && (l_near < h_t)) << 1) | \
                    ((r_far >= r_near) && (r_near < h_t)); \
        if (hit_mask == HM_BOTH) { \
          hit_mask = (l_near < r_near) + 1; \
          trail |= level_bit; \
        } \
      } \
      if (hit_mask == HM_TEST_NODES) { \
        break; \
      } \
      if (hit_mask == HM_RIGHT) { \
        key |= level_bit; \
        node_offset += level_bit; \
      } else { \
        ++node_offset; \
      } \
      int child = (hit_mask & 0x1); \
      if (USE_NODES_IMAGE) { \
        child = read_imageui(nodes_image, NODE_L + child).x; \
      } else { \
        child = nodes[node_index].child[child]; \
      } \
      hit_mask = HM_TEST_NODES; \
      level_bit >>= 1; \
      if (child & TREE_LEAF) { \
        hit_leaf = (child & TREE_LEAF_MASK); \
        break; \
      } else if (child & TREE_SUB_TREE) { \
        tree_offset = (child & TREE_SUB_TREE_MASK); \
        level_bit = LB_NEW_SUB_TREE; \
        node_offset = 0; \
      } \
    } \
    if (hit_leaf != TREE_NO_LEAF_HIT) { \
      uint tri_count = (hit_leaf >> (31 - TREE_LEAF_TRIANGLE_FACTOR)); \
      uint tri_offset = mesh_triangle_offset + \
                        (hit_leaf & \
                        (TREE_LEAF_MASK >> TREE_LEAF_TRIANGLE_FACTOR)); \
      for (uint ti = tri_offset; ti <= tri_offset + tri_count; ++ti) { \
        int tri_pixel = ti * 42; \
        float vertex[3]; \
        float edges[2][3]; \
        float surface_normal[3]; \
        if (USE_TRIANGLES_IMAGE) { \
          vertex[0] = read_imagef(triangles_image, TRI_VERTEX).x; \
          vertex[1] = read_imagef(triangles_image, TRI_VERTEX + 1).x; \
          vertex[2] = read_imagef(triangles_image, TRI_VERTEX + 2).x; \
          edges[0][0] = read_imagef(triangles_image, TRI_EDGES_0).x; \
          edges[0][1] = read_imagef(triangles_image, TRI_EDGES_0 + 1).x; \
          edges[0][2] = read_imagef(triangles_image, TRI_EDGES_0 + 2).x; \
          edges[1][0] = read_imagef(triangles_image, TRI_EDGES_1).x; \
          edges[1][1] = read_imagef(triangles_image, TRI_EDGES_1 + 1).x; \
          edges[1][2] = read_imagef(triangles_image, TRI_EDGES_1 + 2).x; \
          surface_normal[0] = read_imagef(triangles_image, TRI_SURF_N).x; \
          surface_normal[1] = read_imagef(triangles_image, TRI_SURF_N + 1).x; \
          surface_normal[2] = read_imagef(triangles_image, TRI_SURF_N + 2).x; \
        } else { \
          vertex[0] = triangles[ti].vertex[0]; \
          vertex[1] = triangles[ti].vertex[1]; \
          vertex[2] = triangles[ti].vertex[2]; \
          edges[0][0] = triangles[ti].edges[0][0]; \
          edges[0][1] = triangles[ti].edges[0][1]; \
          edges[0][2] = triangles[ti].edges[0][2]; \
          edges[1][0] = triangles[ti].edges[1][0]; \
          edges[1][1] = triangles[ti].edges[1][1]; \
          edges[1][2] = triangles[ti].edges[1][2]; \
          surface_normal[0] = triangles[ti].surface_normal[0]; \
          surface_normal[1] = triangles[ti].surface_normal[1]; \
          surface_normal[2] = triangles[ti].surface_normal[2]; \
        } \
        float c[3], r[3]; \
        c[0] = vertex[0] - O[0]; \
        c[1] = vertex[1] - O[1]; \
        c[2] = vertex[2] - O[2]; \
        r[0] = (D[1] * c[2]) - (D[2] * c[1]); \
        r[1] = (D[2] * c[0]) - (D[0] * c[2]); \
        r[2] = (D[0] * c[1]) - (D[1] * c[0]); \
        float u, v; \
        u = r[0] * edges[1][0] + r[1] * edges[1][1] + \
            r[2] * edges[1][2]; \
        v = r[0] * edges[0][0] + r[1] * edges[0][1] + \
            r[2] * edges[0][2]; \
        float dot = DOT(-D, surface_normal); \
        float t = c[0] * surface_normal[0] + \
                  c[1] * surface_normal[1] + \
                  c[2] * surface_normal[2]; \
        float abs_dot = fabs(dot); \
        float sign_dot = copysign(1.0f, -dot); \
        float rcp_dot = 1.0f / NOT_ZERO(abs_dot); \
        u *= sign_dot; \
        v *= sign_dot; \
        t *= sign_dot * rcp_dot; \
        if ((dot != 0.0f) && (u >= 0.0f) && (v >= 0.0f) && \
            ((u + v) < abs_dot) && (t > 0.0f) && (t < h_t)) { \
          uint material_id; \
          float vertex_uvs[3][2]; \
          if (USE_TRIANGLES_IMAGE) { \
            material_id = read_imageui(triangles_image, TRI_MAT_ID).x; \
            vertex_uvs[0][0] = read_imagef(triangles_image, TRI_UV_0).x; \
            vertex_uvs[0][1] = read_imagef(triangles_image, TRI_UV_0 + 1).x; \
            vertex_uvs[1][0] = read_imagef(triangles_image, TRI_UV_1).x; \
            vertex_uvs[1][1] = read_imagef(triangles_image, TRI_UV_1 + 1).x; \
            vertex_uvs[2][0] = read_imagef(triangles_image, TRI_UV_2).x; \
            vertex_uvs[2][1] = read_imagef(triangles_image, TRI_UV_2 + 1).x; \
          } else { \
            material_id = triangles[ti].material_id; \
            vertex_uvs[0][0] = triangles[ti].vertex_uvs[0][0]; \
            vertex_uvs[0][1] = triangles[ti].vertex_uvs[0][1]; \
            vertex_uvs[1][0] = triangles[ti].vertex_uvs[1][0]; \
            vertex_uvs[1][1] = triangles[ti].vertex_uvs[1][1]; \
            vertex_uvs[2][0] = triangles[ti].vertex_uvs[2][0]; \
            vertex_uvs[2][1] = triangles[ti].vertex_uvs[2][1]; \
          } \
          u *= rcp_dot; \
          v *= rcp_dot; \
          float w = 1.0f - u - v; \
          float uv[2]; \
          uv[0] = w * vertex_uvs[0][0] + u * vertex_uvs[1][0] + \
                  v * vertex_uvs[2][0]; \
          uv[1] = w * vertex_uvs[0][1] + u * vertex_uvs[1][1] + \
                  v * vertex_uvs[2][1]; \
          if ((materials[material_id].glass > 0.0f) || \
              (materials[material_id].emission > 0.0f) || \
              (materials[material_id].double_sided) || (dot > 0.0f)) { \
            float alpha = 1.0f; \
            float alpha_sample = 0.0f; \
            float emission = materials[material_id].emission; \
            if (materials[material_id].map_flags & MAP_FLAG_EMISSION) { \
              float2 emission_uv; \
              emission_uv.x = uv[0]; \
              emission_uv.y = uv[1]; \
              emission_uv.x = \
                mix(materials[material_id].emission_bounds[0] + 0.5f, \
                    materials[material_id].emission_bounds[1] - 0.5f, \
                    emission_uv.x); \
              emission_uv.y = \
                mix(materials[material_id].emission_bounds[2] + 0.5f, \
                    materials[material_id].emission_bounds[3] - 0.5f, \
                    emission_uv.y); \
              const sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE | \
                                        CLK_ADDRESS_REPEAT | CLK_FILTER_LINEAR; \
              float4 emission_map = read_imagef(emission_atlas, sampler, emission_uv); \
              emission *= fmax(fmax(emission_map.x, emission_map.y), \
                               emission_map.z); \
            } \
            if (emission == 0.0f) { \
              alpha = materials[material_id].albedo[3]; \
              PRNG(global_id, b_seeds.w, alpha_sample); \
              if (materials[material_id].map_flags & MAP_FLAG_ALBEDO) { \
                float2 albedo_uv; \
                albedo_uv.x = uv[0]; \
                albedo_uv.y = uv[1]; \
                albedo_uv.x = \
                  mix(materials[material_id].albedo_bounds[0] + 0.5f, \
                      materials[material_id].albedo_bounds[1] - 0.5f, \
                      albedo_uv.x); \
                albedo_uv.y = \
                  mix(materials[material_id].albedo_bounds[2] + 0.5f, \
                      materials[material_id].albedo_bounds[3] - 0.5f, \
                      albedo_uv.y); \
                const sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE | \
                                          CLK_ADDRESS_REPEAT | CLK_FILTER_LINEAR; \
                float4 albedo_map = read_imagef(albedo_atlas, sampler, albedo_uv); \
                alpha *= albedo_map.w; \
              } \
            } \
            if (alpha_sample < alpha) { \
              h_t = t; \
              node_offset = TREE_TRAIL_END; \
              break; \
            } \
          } \
        } \
      } \
      if (node_offset == TREE_TRAIL_END) { \
        break; \
      } \
      hit_leaf = TREE_NO_LEAF_HIT; \
    } \
    if (node_offset == TREE_TRAIL_END) { \
      break; \
    } \
    if (trail == 0x0) { \
      break; \
    } \
    uint shift = 31 - clz(trail & -trail); \
    if (shift >= tree_height) { \
      shift -= tree_height; \
      trail >>= tree_height; \
      key >>= tree_height; \
      tree_height = top_tree_height; \
      tree_offset = top_tree_offset; \
      mesh_triangle_offset = 0; \
    } \
    level_bit = (0x1 << shift); \
    hit_mask = ((key & level_bit) > 0) + 1; \
    trail ^= level_bit; \
    key = (key & (0xFFFFFFFF << (shift + 1))); \
    uint local_key = key & (0xFFFFFFFF >> (32 - tree_height)); \
    node_offset = local_key + ((32 - popcount(local_key)) - \
                               (32 - tree_height) - (shift + 1)); \
  } \
}
///
kernel void Primary(global float* ray_state, 
                    global Material* materials,
                    int use_nodes_image,
                    read_only image1d_buffer_t nodes_image,
                    global TwoNode* nodes,
                    int top_tree_offset,
                    int use_triangles_image,
                    read_only image1d_buffer_t triangles_image,
                    global TreeTriangle* triangles,
                    read_only image2d_t albedo_atlas,
                    read_only image2d_t emission_atlas,
                    Camera camera,
                    uint4  b_seeds,
                    int width, 
                    int height, 
                    int invert_y,
                    int sample)
{
  int global_id = get_global_id(1) * get_global_size(0) + get_global_id(0); 
  int ray_count = width * height;
  if (global_id >= ray_count) {
    return;
  }
  float o[3], d[3];
  float pixel_sample[2];
  PRNG(global_id, b_seeds.x, pixel_sample[0]);
  PRNG(global_id, b_seeds.y, pixel_sample[1]);
  int x_pixel = global_id % width;
  int y_pixel = global_id / width;
  if (invert_y) {
    y_pixel = (height - 1) - y_pixel;
  }
  float p[3] = { camera.root_pixel[0] + ((x_pixel + pixel_sample[0]) * 
                                         camera.pixel_offset[0]),
                 camera.root_pixel[1] + ((y_pixel + pixel_sample[1]) * 
                                         camera.pixel_offset[1]), 
                 1.0f };
  p[0] *= camera.focal_depth;
  p[1] *= camera.focal_depth;
  p[2] *= camera.focal_depth;
  float pt[3];
  TRANSFORM_X(p, camera.transform, pt);
  TRANSFORM_Y(p, camera.transform, pt);
  TRANSFORM_Z(p, camera.transform, pt);
  float aperture_sample[4];
  PRNG(global_id, b_seeds.z, aperture_sample[0]);
  PRNG(global_id, b_seeds.w, aperture_sample[1]);
  float aperture_o[3];
  CONCENTRIC_HEMI(aperture_sample[0], aperture_sample[1], aperture_o);
  aperture_o[0] *= camera.aperture_size;
  aperture_o[1] *= camera.aperture_size;
  aperture_o[2] = 0.0f;
  TRANSFORM_X(aperture_o, camera.transform, o);
  TRANSFORM_Y(aperture_o, camera.transform, o);
  TRANSFORM_Z(aperture_o, camera.transform, o);
  d[0] = pt[0] - o[0];
  d[1] = pt[1] - o[1];
  d[2] = pt[2] - o[2];
  NORMALIZE(d);
  ray_state[RS_THROUGHPUT_R] = 1.0f;
  ray_state[RS_THROUGHPUT_G] = 1.0f;
  ray_state[RS_THROUGHPUT_B] = 1.0f;
  if (sample == 0) {
    ray_state[RS_COLOR_R] = 0.0f;
    ray_state[RS_COLOR_G] = 0.0f;
    ray_state[RS_COLOR_B] = 0.0f;
  }
	TRACE(use_nodes_image, use_triangles_image, true);
	ray_state[RS_LAST_BRDF_PDF] = DIRAC_PDF;
  return;
}
///
kernel void Secondary(global float* ray_state, 
                      global Material* materials,
                      int use_nodes_image,
                      read_only image1d_buffer_t nodes_image,
                      global TwoNode* nodes,
                      int top_tree_offset,
                      int use_triangles_image,
                      read_only image1d_buffer_t triangles_image,
                      global TreeTriangle* triangles,
                      read_only image2d_t albedo_atlas,
                      read_only image2d_t emission_atlas,
                      uint4  b_seeds,
                      int width, 
                      int height)
{
  int global_id = get_global_id(1) * get_global_size(0) + get_global_id(0); 
  int ray_count = width * height;
  if (global_id >= ray_count) {
    return;
  }
  int state = ray_state[RS_STATE];
  if (state != STATE_SECONDARY) {
    return;
  }
  float o[3], d[3];
  d[0] = ray_state[RS_DIR_X];
  d[1] = ray_state[RS_DIR_Y];
  d[2] = ray_state[RS_DIR_Z];
  o[0] = ray_state[RS_ORIGIN_X] + (d[0] * EPSILON);
  o[1] = ray_state[RS_ORIGIN_Y] + (d[1] * EPSILON);
  o[2] = ray_state[RS_ORIGIN_Z] + (d[2] * EPSILON);
  TRACE(use_triangles_image, use_nodes_image, false);
}

///
kernel void EnvSky(global float* ray_state,
                   read_only image2d_t sun_table,
                   Sun sun,
                   int width,
                   int height,
                   int bounce)
{
  int global_id = get_global_id(0);
  int ray_count = width * height;
  if (global_id >= ray_count) {
    return;
  }
  int state = ray_state[RS_STATE];
  if (state != STATE_ENVIRONMENT) {
    return;
  }
  float brdf_pdf = ray_state[RS_LAST_BRDF_PDF];
  float d_i[3];
  d_i[0] = ray_state[RS_DIR_X];
  d_i[1] = ray_state[RS_DIR_Y];
  d_i[2] = ray_state[RS_DIR_Z];
  float s = -6360.5f * d_i[1];
  float mu = -s * 0.00015722034f;
  float nu = d_i[0] * sun.direction[0] + d_i[1] * sun.direction[1] + 
             d_i[2] * sun.direction[2];
  float mus = (6360.5f * sun.direction[1]) * 0.00015722034f;
  float rmu = 6360.5f * mu;
  float delta = rmu * rmu - 6360.25f;
  float4 cst;
  if ((rmu < 0.0f) && (delta > 0.0f)) {
    cst = (float4)(1.0f, 0.0f, 0.0f, 0.484375f);
  } else {
    cst = (float4)(-1.0f, 766800.000001f, 875.67117116f, 0.515625f);
  }
  float umu = cst.w + (rmu * cst.x + native_sqrt(delta + cst.y)) /
              (79.7511755399f + cst.z) * 0.4921875f;
  float umus = 0.015625f + 
               (atan(fmax(mus, -0.1975f) * 5.34962349919f) * 0.90909090f + 
               0.74f) * 0.484375f;
  float lerp_value = (nu + 1.0f) * 3.5f;
  float unu = floor(lerp_value);
  lerp_value = lerp_value - unu;
  float2 uv0 = (float2)((unu + umus) * 0.125f, umu);
  float2 uv1 = (float2)((unu + umus + 1.0f) * 0.125f, umu);
	const sampler_t sampler = CLK_NORMALIZED_COORDS_TRUE |
                            CLK_ADDRESS_REPEAT | CLK_FILTER_LINEAR;
  float4 raymie = fmax((read_imagef(sun_table, sampler, uv0) * (1.0f - lerp_value)) +
                       (read_imagef(sun_table, sampler, uv1) * lerp_value), 0.0f);
  float pr = 0.05968310365f * (1.0f + nu * nu);
  float pm = 0.04297183463f * pow(1.64f - 1.6f * nu, -1.5f) * 
             (1.0f + nu * nu) * 0.378378378f;
  float4 beta_r = (float4)(0.0058f, 0.0135f, 0.0331f, 1.0f);
  float4 mie = raymie * raymie.w / (raymie.x) * (beta_r.x / beta_r) * \
                (1.0f + (d_i[1] * 3.0f));
  float4 result = fmax(raymie * pr + mie * pm, (float4)(0.0f, 0.0f, 0.0f, 0.0f)) * 
                  30.0f * sun.sky_intensity;
  float sun_delta = sun.direction[0] * d_i[0] + sun.direction[1] * d_i[1] + 
                    sun.direction[2] * d_i[2];
  float color[3] = { result.x, result.y, result.z };
  float mis = 1.0f;
  if ((brdf_pdf != -1.0f) && (sun_delta >= SUN_COS_THETA)) {
    float sun_pdf = 1.0f / (TAU * (1.0f - SUN_COS_THETA));
    if (bounce > 0) {
      if (brdf_pdf < DIRAC_PDF) {
        if (sun.sun_intensity > 0.0f) {
          mis = MIS(brdf_pdf, sun_pdf);
        } else {
          sun.sun_intensity *= 0.05f;
        }
      } else {
        if (bounce > 5) {
          ray_state[RS_STATE] = STATE_END;
          return;
        }
        sun.sun_intensity *= 0.05f;
      }
    } else {
      sun.sun_intensity *= 0.05f;
    }
    float horizon_falloff = fabs(d_i[1] - 0.075f) - (d_i[1] - 0.075f);
    horizon_falloff = pow(horizon_falloff, 4.0f);
    float hfo = horizon_falloff * 1000.0f;
    sun.sun_intensity *= 1.0f + (sun.direction[1] * 4.0f);
    float multiplier = clamp(1.0f / NOT_ZERO(hfo), 1.0f,
                             fmax(5.0f, sun.sun_intensity * 100.0f));
    color[0] *= multiplier;
    color[1] *= multiplier;
    color[2] *= multiplier;
  } else if (bounce > 0) {
    float horizon_bias = fmin(0.5f, 
                              (fmax(0.0f, sun.direction[1] - 0.1f) * 10.0f));
    float max_component = fmax(color[0], fmax(color[1], color[2]));
    color[0] = mix(color[0], max_component * 1.0f, horizon_bias);
    color[1] = mix(color[1], max_component * 0.98f, horizon_bias); 
    color[2] = mix(color[2], max_component * 0.73f, horizon_bias); 
    color[0] *= sun.color[0];
    color[1] *= sun.color[1];
    color[2] *= sun.color[2];
  }
  ray_state[RS_COLOR_R] += ray_state[RS_THROUGHPUT_R] * color[0] * mis;
  ray_state[RS_COLOR_G] += ray_state[RS_THROUGHPUT_G] * color[1] * mis;
  ray_state[RS_COLOR_B] += ray_state[RS_THROUGHPUT_B] * color[2] * mis;
  ray_state[RS_STATE] = STATE_END;
  return;
}
///
kernel void EnvMap(global float* ray_state,
                   read_only image2d_t environment_map,
                   EnvironmentProperties environment_properties,
                   int width,
                   int height,
                   int bounce)
{
  int global_id = get_global_id(0); 
  int ray_count = width * height;
  if (global_id >= ray_count) {
    return;
  }
  int state = ray_state[RS_STATE];
  if (state != STATE_ENVIRONMENT) {
    return;
  }
  float d_i[3];
  d_i[0] = ray_state[RS_DIR_X];
  d_i[1] = ray_state[RS_DIR_Y];
  d_i[2] = ray_state[RS_DIR_Z];
  NORMALIZE(d_i);
  float2 uv = (float2)(1.0f + atan2(d_i[0], -d_i[2]), acos(-d_i[1]) * INV_PI);
  uv.x *= INV_TAU;
  uv.x += environment_properties.rotation;
  const sampler_t sampler = CLK_NORMALIZED_COORDS_TRUE |
                            CLK_ADDRESS_REPEAT | CLK_FILTER_LINEAR;
  float4 color = read_imagef(environment_map, sampler, uv);
	color.x *= environment_properties.intensity;
	color.y *= environment_properties.intensity;
	color.z *= environment_properties.intensity;
  ray_state[RS_COLOR_R] += ray_state[RS_THROUGHPUT_R] * color.x;
  ray_state[RS_COLOR_G] += ray_state[RS_THROUGHPUT_G] * color.y;
  ray_state[RS_COLOR_B] += ray_state[RS_THROUGHPUT_B] * color.z;
  ray_state[RS_STATE] = STATE_END;
  return;

}
///
kernel void Emission(global float* ray_state,
                     global Material* materials,
                     int use_triangles_image,
                     read_only image1d_buffer_t triangles_image,
                     global TreeTriangle* triangles,
                     read_only image2d_t emission_atlas,
                     int emitter_count,
                     int width,
                     int height,
                     int bounce)
{
  int global_id = get_global_id(0);
  int ray_count = width * height;
  if (global_id >= ray_count) {
    return;
  }
  int state = ray_state[RS_STATE];
  if (state != STATE_EMISSIVE) {
    return;
  }
  float d[3];
  d[0] = ray_state[RS_DIR_X];
  d[1] = ray_state[RS_DIR_Y];
  d[2] = ray_state[RS_DIR_Z];
  float u = ray_state[RS_U];
  float v = ray_state[RS_V];
  float w = 1.0f - u - v;
  int tri_index = ray_state[RS_TRI_INDEX];
  int tri_pixel = tri_index * 42;
  float vn[3];
  if (use_triangles_image) {
    vn[0] = w * read_imagef(triangles_image, TRI_VN_0).x + 
            u * read_imagef(triangles_image, TRI_VN_1).x + 
            v * read_imagef(triangles_image, TRI_VN_2).x;
    vn[1] = w * read_imagef(triangles_image, TRI_VN_0 + 1).x + 
            u * read_imagef(triangles_image, TRI_VN_1 + 1).x + 
            v * read_imagef(triangles_image, TRI_VN_2 + 1).x;
    vn[2] = w * read_imagef(triangles_image, TRI_VN_0 + 2).x + 
            u * read_imagef(triangles_image, TRI_VN_1 + 2).x + 
            v * read_imagef(triangles_image, TRI_VN_2 + 2).x;
  } else {
    vn[0] = w * triangles[tri_index].vertex_normals[0][0] +
            u * triangles[tri_index].vertex_normals[1][0] + 
            v * triangles[tri_index].vertex_normals[2][0];
    vn[1] = w * triangles[tri_index].vertex_normals[0][1] +
            u * triangles[tri_index].vertex_normals[1][1] + 
            v * triangles[tri_index].vertex_normals[2][1];
    vn[2] = w * triangles[tri_index].vertex_normals[0][2] +
            u * triangles[tri_index].vertex_normals[1][2] + 
            v * triangles[tri_index].vertex_normals[2][2];
  }
  NORMALIZE(vn); 
  float dot_ni = DOT(-d, vn);
  if (dot_ni <= 0.0f) {
    ray_state[RS_STATE] = STATE_END;
    return;
  } else {
    uint mat_id;
    float uv[2];
    float inverse_area;
    if (use_triangles_image) {
      mat_id = read_imageui(triangles_image, TRI_MAT_ID).x;
      uv[0] = w * read_imagef(triangles_image, TRI_UV_0).x +
              u * read_imagef(triangles_image, TRI_UV_1).x +
              v * read_imagef(triangles_image, TRI_UV_2).x;
      uv[1] = w * read_imagef(triangles_image, TRI_UV_0 + 1).x +
              u * read_imagef(triangles_image, TRI_UV_1 + 1).x +
              v * read_imagef(triangles_image, TRI_UV_2 + 1).x;
      inverse_area = read_imagef(triangles_image, TRI_INV_AREA).x;
    } else {
      mat_id = triangles[tri_index].material_id;
      uv[0] = w * triangles[tri_index].vertex_uvs[0][0] +
              u * triangles[tri_index].vertex_uvs[1][0] +
              v * triangles[tri_index].vertex_uvs[2][0];
      uv[1] = w * triangles[tri_index].vertex_uvs[0][1] +
              u * triangles[tri_index].vertex_uvs[1][1] +
              v * triangles[tri_index].vertex_uvs[2][1];
      inverse_area = triangles[tri_index].inverse_area; 
    }
    float t = ray_state[RS_HIT_DISTANCE];
    float t_2 = t * t;
    float emitter_color[3];
    float intensity = materials[mat_id].emission * 25.0f;
    if (materials[mat_id].map_flags & MAP_FLAG_EMISSION) {
      float2 emission_uv;
      emission_uv.x = uv[0];
      emission_uv.y = uv[1];
      emission_uv.x = mix(materials[mat_id].emission_bounds[0] + 0.5f,
                           materials[mat_id].emission_bounds[1] - 0.5f, 
                           emission_uv.x);
      emission_uv.y = mix(materials[mat_id].emission_bounds[2] + 0.5f,
                           materials[mat_id].emission_bounds[3] - 0.5f, 
                           emission_uv.y);
      const sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE |
                                CLK_ADDRESS_REPEAT | CLK_FILTER_LINEAR;
      float4 emission_map = read_imagef(emission_atlas, sampler, emission_uv);
      emitter_color[0] = emission_map.x * intensity;
      emitter_color[1] = emission_map.y * intensity;
      emitter_color[2] = emission_map.z * intensity;
    } else {
      emitter_color[0] = materials[mat_id].albedo[0] * intensity;
      emitter_color[1] = materials[mat_id].albedo[1] * intensity;
      emitter_color[2] = materials[mat_id].albedo[2] * intensity;
    }
    float brdf_pdf = ray_state[RS_LAST_BRDF_PDF];
    if (brdf_pdf == -1.0f) {
      ray_state[RS_STATE] = STATE_END;
      return;
    }
    float emitter_prob = 1.0f / (float)emitter_count; 
    float emitter_pdf = inverse_area *
                        (t_2 / fabs(dot_ni)) * emitter_prob;
    float mis = 1.0f;
    if (bounce > 0) {
      if (brdf_pdf < DIRAC_PDF) {
        mis = MIS(brdf_pdf, emitter_pdf);
      } else if (bounce > 5) {
        ray_state[RS_STATE] = STATE_END;
        return;
      }
    }
    ray_state[RS_COLOR_R] += ray_state[RS_THROUGHPUT_R] * emitter_color[0] * mis;
    ray_state[RS_COLOR_G] += ray_state[RS_THROUGHPUT_G] * emitter_color[1] * mis;
    ray_state[RS_COLOR_B] += ray_state[RS_THROUGHPUT_B] * emitter_color[2] * mis;
    ray_state[RS_STATE] = STATE_END;
    return;
  }
}
///
kernel void Sample(global float* ray_state,
                   global Material* materials,
                   int use_triangles_image,
                   read_only image1d_buffer_t triangles_image,
                   global TreeTriangle* triangles,
                   read_only image2d_t albedo_atlas,
                   read_only image2d_t normal_atlas,
                   read_only image2d_t metallic_atlas,
                   read_only image2d_t emission_atlas,
                   read_only image2d_t sun_table,
                   Sun sun,
                   read_only image2d_t environment_map,
                   EnvironmentProperties environment_properties,
                   global EmitterIndex* emitters,
                   int emitter_count,
                   global PortalIndex* portals,
                   int portal_count,
                   uint4 b_seeds_0,
                   uint4 b_seeds_1,
                   uint4 b_seeds_2,
                   uint4 b_seeds_3,
                   int width,
                   int height,
                   int sample_sun)
{
  int global_id = get_global_id(0); 
  int ray_count = width * height;
  if (global_id >= ray_count) {
    return;
  }
  int state = ray_state[RS_STATE];
  if (state == STATE_END) {
    return;
  }
  float last_brdf_pdf;
  float new_brdf_pdf;
  float sn[3];
  float vn[3];
  float n[3];
  float n_tangent[3];
  float n_binormal[3];
  float ld_i[3];
  float old_throughput[3];
  float albedo[3];
  float metallic;
  float smoothness;
  bool glass_material;
  {
    float u = ray_state[RS_U];
    float v = ray_state[RS_V];
    float w = 1.0f - u - v;
    uint tri_index = ray_state[RS_TRI_INDEX];
    int tri_pixel = tri_index * 42;
    uint mat_id;
    float uv[2];
    if (use_triangles_image) {
      mat_id = read_imageui(triangles_image, TRI_MAT_ID).x;
      sn[0] = read_imagef(triangles_image, TRI_SURF_N).x;
      sn[1] = read_imagef(triangles_image, TRI_SURF_N + 1).x;
      sn[2] = read_imagef(triangles_image, TRI_SURF_N + 2).x;
      uv[0] = w * read_imagef(triangles_image, TRI_UV_0).x +
              u * read_imagef(triangles_image, TRI_UV_1).x +
              v * read_imagef(triangles_image, TRI_UV_2).x;
      uv[1] = w * read_imagef(triangles_image, TRI_UV_0 + 1).x +
              u * read_imagef(triangles_image, TRI_UV_1 + 1).x +
              v * read_imagef(triangles_image, TRI_UV_2 + 1).x;
      vn[0] = w * read_imagef(triangles_image, TRI_VN_0).x + 
              u * read_imagef(triangles_image, TRI_VN_1).x + 
              v * read_imagef(triangles_image, TRI_VN_2).x;
      vn[1] = w * read_imagef(triangles_image, TRI_VN_0 + 1).x + 
              u * read_imagef(triangles_image, TRI_VN_1 + 1).x + 
              v * read_imagef(triangles_image, TRI_VN_2 + 1).x;
      vn[2] = w * read_imagef(triangles_image, TRI_VN_0 + 2).x + 
              u * read_imagef(triangles_image, TRI_VN_1 + 2).x + 
              v * read_imagef(triangles_image, TRI_VN_2 + 2).x;
    } else {
      mat_id = triangles[tri_index].material_id;
      sn[0] = triangles[tri_index].surface_normal[0];
      sn[1] = triangles[tri_index].surface_normal[1];
      sn[2] = triangles[tri_index].surface_normal[2];
      uv[0] = w * triangles[tri_index].vertex_uvs[0][0] +
              u * triangles[tri_index].vertex_uvs[1][0] +
              v * triangles[tri_index].vertex_uvs[2][0];
      uv[1] = w * triangles[tri_index].vertex_uvs[0][1] +
              u * triangles[tri_index].vertex_uvs[1][1] +
              v * triangles[tri_index].vertex_uvs[2][1];
      vn[0] = w * triangles[tri_index].vertex_normals[0][0] +
              u * triangles[tri_index].vertex_normals[1][0] + 
              v * triangles[tri_index].vertex_normals[2][0];
      vn[1] = w * triangles[tri_index].vertex_normals[0][1] +
              u * triangles[tri_index].vertex_normals[1][1] + 
              v * triangles[tri_index].vertex_normals[2][1];
      vn[2] = w * triangles[tri_index].vertex_normals[0][2] +
              u * triangles[tri_index].vertex_normals[1][2] + 
              v * triangles[tri_index].vertex_normals[2][2];
    }
    glass_material = (materials[mat_id].glass > 0.0f);
    float d_i[3];
    d_i[0] = -ray_state[RS_DIR_X];
    d_i[1] = -ray_state[RS_DIR_Y];
    d_i[2] = -ray_state[RS_DIR_Z];
    albedo[0] = materials[mat_id].albedo[0];
    albedo[1] = materials[mat_id].albedo[1];
    albedo[2] = materials[mat_id].albedo[2];
    if (materials[mat_id].map_flags & MAP_FLAG_ALBEDO) {
      float2 albedo_uv;
      albedo_uv.x = uv[0];
      albedo_uv.y = uv[1];
      albedo_uv.x = mix(materials[mat_id].albedo_bounds[0] + 0.5f,
                         materials[mat_id].albedo_bounds[1] - 0.5f, 
                         albedo_uv.x);
      albedo_uv.y = mix(materials[mat_id].albedo_bounds[2] + 0.5f,
                         materials[mat_id].albedo_bounds[3] - 0.5f, 
                         albedo_uv.y);
      const sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE |
                                CLK_ADDRESS_REPEAT | CLK_FILTER_LINEAR;
      float4 albedo_map = read_imagef(albedo_atlas, sampler, albedo_uv);
      albedo[0] *= albedo_map.x;
      albedo[1] *= albedo_map.y;
      albedo[2] *= albedo_map.z;
    }
    if (materials[mat_id].map_flags & MAP_FLAG_NORMAL) {
      float2 normal_uv;
      normal_uv.x = uv[0];
      normal_uv.y = uv[1];
      normal_uv.x = mix(materials[mat_id].normal_bounds[0] + 0.5f,
                         materials[mat_id].normal_bounds[1] - 0.5f, 
                         normal_uv.x);
      normal_uv.y = mix(materials[mat_id].normal_bounds[2] + 0.5f,
                         materials[mat_id].normal_bounds[3] - 0.5f, 
                         normal_uv.y);
      const sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE |
                                CLK_ADDRESS_REPEAT | CLK_FILTER_LINEAR;
      float4 normal_map = read_imagef(normal_atlas, sampler, normal_uv);
      float vn_tangent[3], vn_binormal[3];
      GET_VT_BASIS(vn, vn_tangent, vn_binormal);
      float ln[3];
      ln[0] = normal_map.w * 2.0f - 1.0f;
      ln[1] = normal_map.y * 2.0f - 1.0f;
      ln[2] = native_sqrt(1.0f - 
              clamp(ln[0] * ln[0] + ln[1] * ln[1], 0.0f, 1.0f));
      ROTATE_TO_BASIS(ln, vn, vn_tangent, vn_binormal, n);
    } else {
      n[0] = vn[0];
      n[1] = vn[1];
      n[2] = vn[2];
    }
    NORMALIZE(n);
    if (!glass_material && (DOT(d_i, sn) < 0.0f)) {
      n[0] = -n[0];
      n[1] = -n[1];
      n[2] = -n[2];
    }
    metallic = materials[mat_id].metallic;
    smoothness = materials[mat_id].smoothness;
    if (materials[mat_id].map_flags & MAP_FLAG_METALLIC) {
      float2 metallic_uv;
      metallic_uv.x = uv[0];
      metallic_uv.y = uv[1];
      metallic_uv.x = mix(materials[mat_id].metallic_bounds[0] + 0.5f,
                           materials[mat_id].metallic_bounds[1] - 0.5f, 
                           metallic_uv.x);
      metallic_uv.y = mix(materials[mat_id].metallic_bounds[2] + 0.5f,
                           materials[mat_id].metallic_bounds[3] - 0.5f, 
                           metallic_uv.y);
      const sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE |
                                CLK_ADDRESS_REPEAT | CLK_FILTER_LINEAR;
      float4 metallic_map = read_imagef(metallic_atlas, sampler, metallic_uv);
      metallic *= metallic_map.x;
      smoothness *= metallic_map.w;
    }
		float roughness[2] = { 1.0f - smoothness,
                           1.0f - smoothness };
    GET_BASIS(n, n_tangent, n_binormal);
    RELATIVE_TO_BASIS(d_i, n, n_tangent, n_binormal, ld_i);
    float ggx_pdf;
		float ln_facet[3];
		if (smoothness == 1.0f) {
			ln_facet[0] = 0.0f;
			ln_facet[1] = 0.0f;
			ln_facet[2] = 1.0f;
      ggx_pdf = DIRAC_PDF;
    } else {
			float ggx_samples[2];
		  PRNG(global_id, b_seeds_0.x, ggx_samples[0]);
		  PRNG(global_id, b_seeds_0.y, ggx_samples[1]);
			GGX_SAMPLE(ld_i, roughness, ggx_samples[0], ggx_samples[1], ln_facet); 
      float g1, eval;
      SMITH_G1(ld_i, ln_facet, roughness, g1);
      GGX_EVAL(ln_facet, roughness, eval);
      ggx_pdf = g1 * fabs(DOT(ld_i, ln_facet)) * eval / fabs(ld_i[2]);
    }
    if (ld_i[2] == 0.0f) {
      ggx_pdf = 0.0f;
    }
    last_brdf_pdf = ray_state[RS_LAST_BRDF_PDF];
    float coating_dropoff = fmin(smoothness * 2.0f, 1.0f);
    float brdf_color[3];
    float ld_o[3];
    float sample_dice;
    PRNG(global_id, b_seeds_0.z, sample_dice);
    if (glass_material || (sample_dice >= metallic)) {
      // Dielectric interface
      float fresnel, cos_theta_t;
      DIELECTRIC_FRESNEL(ld_i[2], 1.5f, cos_theta_t, fresnel);
      float reflect_sample;
      PRNG(global_id, b_seeds_0.w, reflect_sample);
      if (reflect_sample <= (fresnel * coating_dropoff)) {
        // Reflect
        float dot_ni = DOT(ln_facet, ld_i);
        ld_o[0] = 2.0f * ln_facet[0] * dot_ni - ld_i[0];
        ld_o[1] = 2.0f * ln_facet[1] * dot_ni - ld_i[1];
        ld_o[2] = 2.0f * ln_facet[2] * dot_ni - ld_i[2];
        ld_o[2] = fabs(ld_o[2]);
        NORMALIZE(ld_o);
        if ((ggx_pdf == 0.0f) || (ld_o[2] <= 0.0f)) {
          brdf_color[0] = 0.0f;
          brdf_color[1] = 0.0f;
          brdf_color[2] = 0.0f;
          new_brdf_pdf = -1.0f;
        } else {
          brdf_color[0] = 1.0f;
          brdf_color[1] = 1.0f;
          brdf_color[2] = 1.0f;
          /*
          float weight;
          SMITH_G1(ld_o, ln_facet, roughness, weight);
          brdf_color[0] *= weight;
          brdf_color[1] *= weight;
          brdf_color[2] *= weight; */
          if (last_brdf_pdf != DIRAC_PDF) {
            new_brdf_pdf = -1.0f;
          } else {
            if (ggx_pdf == DIRAC_PDF) {
              new_brdf_pdf = DIRAC_PDF;
            } else {
              new_brdf_pdf = ggx_pdf;
              new_brdf_pdf *= fresnel * coating_dropoff;
              float dwh_dwo = 1.0f / (4.0f * DOT(ld_o, ln_facet));
              new_brdf_pdf *= fabs(dwh_dwo);
            }
          }
        }
      } else {
        // Transmit 
        if (glass_material) {
          // Refract
          float ior = (cos_theta_t  < 0.0f) ? (1.0f / 1.5f) : 1.5f;
          float ior_2 = ior * ior;
          float dot_ni = DOT(ln_facet, ld_i);
          float c = dot_ni * ior + cos_theta_t;
          ld_o[0] = ln_facet[0] * c - ld_i[0] * ior;
          ld_o[1] = ln_facet[1] * c - ld_i[1] * ior;
          ld_o[2] = ln_facet[2] * c - ld_i[2] * ior;
          copysign(ld_o[2], -ld_o[1]);
          NORMALIZE(ld_o);
          if ((cos_theta_t == 0.0f) || (ld_i[2] * ld_o[2] >= 0.0f)) {
            brdf_color[0] = 0.0f;
            brdf_color[1] = 0.0f;
            brdf_color[2] = 0.0f;
            new_brdf_pdf = -1.0f;
          } else {
            brdf_color[0] = albedo[0] * ior_2;
            brdf_color[1] = albedo[1] * ior_2;
            brdf_color[2] = albedo[2] * ior_2;
            /*
            float weight;
            SMITH_G1(ld_o, ln_facet, roughness, weight);
            brdf_color[0] *= weight;
            brdf_color[1] *= weight;
            brdf_color[2] *= weight; */
            if (last_brdf_pdf != DIRAC_PDF) {
              new_brdf_pdf = -1.0f;
            } else {
              if (ggx_pdf == DIRAC_PDF) {
                new_brdf_pdf = DIRAC_PDF;
              } else {
                float dot_no = DOT(ln_facet, ld_o);
                new_brdf_pdf = ggx_pdf;
                float sqrt_denom = dot_ni + ior * dot_no; 
                float dwh_dwo = (ior_2 * dot_no) / (sqrt_denom * sqrt_denom);
                new_brdf_pdf *= fabs(dwh_dwo);
              }
            }
          }
        } else {
          // Diffuse 
          ln_facet[0] = 0.0f;
          ln_facet[1] = 0.0f;
          ln_facet[2] = 1.0f;
          float ln_tangent[3];
          float ln_binormal[3];
          GET_BASIS(ln_facet, ln_tangent, ln_binormal);
          float hemi_sample[2];
          PRNG(global_id, b_seeds_1.x, hemi_sample[0]);
          PRNG(global_id, b_seeds_1.y, hemi_sample[1]);
          float hemi_d[3];
          CONCENTRIC_HEMI(hemi_sample[0], hemi_sample[1], hemi_d);
          ROTATE_TO_BASIS(hemi_d, ln_facet, ln_tangent, ln_binormal, ld_o);
          ld_o[2] = fabs(ld_o[2]);
          brdf_color[0] = albedo[0];
          brdf_color[1] = albedo[1];
          brdf_color[2] = albedo[2];
          new_brdf_pdf = INV_PI * ld_o[2];
        }
      }
    } else {
      // Conductor
      float dot_ni = DOT(ln_facet, ld_i);
      ld_o[0] = 2.0f * ln_facet[0] * dot_ni - ld_i[0];
      ld_o[1] = 2.0f * ln_facet[1] * dot_ni - ld_i[1];
      ld_o[2] = 2.0f * ln_facet[2] * dot_ni - ld_i[2];
      ld_o[2] = fabs(ld_o[2]);
      NORMALIZE(ld_o);
      if ((ggx_pdf == 0.0f) || (ld_o[2] <= 0.0f)) {
        brdf_color[0] = 0.0f;
        brdf_color[1] = 0.0f;
        brdf_color[2] = 0.0f;
        new_brdf_pdf = -1.0f;
      } else {
        float f;
        CONDUCTOR_FRESNEL(dot_ni, 1.5f, 3.0f, f);
        brdf_color[0] = albedo[0] * f;
        brdf_color[1] = albedo[1] * f;
        brdf_color[2] = albedo[2] * f;
        /*
        float weight;
        SMITH_G1(ld_o, ln_facet, roughness, weight);
        brdf_color[0] *= weight;
        brdf_color[1] *= weight;
        brdf_color[2] *= weight; */
        if (last_brdf_pdf != DIRAC_PDF) {
          new_brdf_pdf = -1.0f;
        } else {
          if (ggx_pdf == DIRAC_PDF) {
            new_brdf_pdf = DIRAC_PDF;
          } else {
            new_brdf_pdf = ggx_pdf / (4.0f * DOT(ld_o, ln_facet));
          }
        }
      }
    }
    float d_o[3];
    ROTATE_TO_BASIS(ld_o, n, n_tangent, n_binormal, d_o);
    ray_state[RS_LAST_BRDF_PDF] = new_brdf_pdf;
    old_throughput[0] = ray_state[RS_THROUGHPUT_R];
    old_throughput[1] = ray_state[RS_THROUGHPUT_G];
    old_throughput[2] = ray_state[RS_THROUGHPUT_B];
    ray_state[RS_THROUGHPUT_R] = old_throughput[0] * brdf_color[0];
    ray_state[RS_THROUGHPUT_G] = old_throughput[1] * brdf_color[1];
    ray_state[RS_THROUGHPUT_B] = old_throughput[2] * brdf_color[2];
    ray_state[RS_DIR_X] = d_o[0];
    ray_state[RS_DIR_Y] = d_o[1];
    ray_state[RS_DIR_Z] = d_o[2];
    ray_state[RS_STATE] = STATE_SECONDARY;
  }
  float sun_d[3];
  sun_d[0] = FLT_MAX;
  if (sample_sun > 0) {
    float theta, z;
    PRNG(global_id, b_seeds_1.z, theta);
    PRNG(global_id, b_seeds_1.w, z);
    theta *= TAU;
    z = (z * (1.0f - SUN_COS_THETA)) + SUN_COS_THETA;
    float z2 = native_sqrt(1.0f - (z * z));
    float sun_ld_o[3];
    sun_ld_o[0] = z2 * native_cos(theta);
    sun_ld_o[1] = z2 * native_sin(theta);
    sun_ld_o[2] = z;
    float sun_tangent[3];
    float sun_binormal[3];
    GET_BASIS(sun.direction, sun_tangent, sun_binormal);
    ROTATE_TO_BASIS(sun_ld_o, sun.direction, sun_tangent, sun_binormal, 
                    sun_d);
    sun_d[1] = fmax(0.01f, sun_d[1]);
    NORMALIZE(sun_d);
    float cos_at_tri = DOT(sun_d, n);
    if (cos_at_tri <= 0.0f) {
      sun_d[0] = FLT_MAX;
    } else {
      float sun_color[3];
      float brdf_color[3];
      float brdf_pdf;
      float sun_pdf = 1.0f / (TAU * (1.0f - SUN_COS_THETA));
      float s = -6360.5f * sun_d[1];
      float mu = -s * 0.00015722034f;
      float nu = sun_d[0] * sun.direction[0] + sun_d[1] * sun.direction[1] + 
                 sun_d[2] * sun.direction[2];
      float mus = (6360.5f * sun.direction[1]) * 0.00015722034f;
      float rmu = 6360.5f * mu;
      float delta = rmu * rmu - 6360.25f;
      float4 cst;
      if ((rmu < 0.0f) && (delta > 0.0f)) {
        cst = (float4)(1.0f, 0.0f, 0.0f, 0.484375f);
      } else {
        cst = (float4)(-1.0f, 766800.000001f, 875.67117116f, 0.515625f);
      }
      float umu = cst.w + (rmu * cst.x + native_sqrt(delta + cst.y)) / 
                  (79.7511755399f + cst.z) * 0.4921875f;
      float umus = 0.015625f + 
                   (atan(fmax(mus, -0.1975f) * 5.34962349919f) * 0.90909090f + 
                   0.74f) * 0.484375f;
      float lerp_value = (nu + 1.0f) * 3.5f;
      float unu = floor(lerp_value);
      lerp_value = lerp_value - unu;
			float2 uv0 = (float2)((unu + umus) * 0.125f, umu);
			float2 uv1 = (float2)((unu + umus + 1.0f) * 0.125f, umu);
      const sampler_t sampler = CLK_NORMALIZED_COORDS_TRUE |
                                CLK_ADDRESS_REPEAT | CLK_FILTER_LINEAR;
      float4 raymie = fmax((read_imagef(sun_table, sampler, uv0) * (1.0f - lerp_value)) +
                           (read_imagef(sun_table, sampler, uv1) * lerp_value), 0.0f);
      float pr = 0.05968310365f * (1.0f + nu * nu);
      float pm = 0.04297183463f * pow(1.64f - 1.6f * nu, -1.5f) * 
                 (1.0f + nu * nu) * 0.378378378f;
      float4 beta_r = (float4)(0.0058f, 0.0135f, 0.0331f, 1.0f);
      float4 mie = raymie * raymie.w / NOT_ZERO(raymie.x) * (beta_r.x / beta_r);
      float4 sun_result = fmax(raymie * pr + mie * pm, (float4)(0.0f, 0.0f, 0.0f, 0.0f)) * 
                          30.0f * sun.sky_intensity;
      sun_result.x *= sun.color[0];
      sun_result.y *= sun.color[1];
      sun_result.z *= sun.color[2];
      sun.sun_intensity *= 1.0f + (sun.direction[1] * 4.0f);
      float horizon_falloff = fabs(sun_d[1] - 0.075f) - (sun_d[1] - 0.075f);
      horizon_falloff = pow(horizon_falloff, 4.0f);
      float hfo = horizon_falloff * 1000.0f;
      sun.sun_intensity *= 1.0f + (sun.direction[1] * 4.0f);
      float multiplier = clamp(1.0f / NOT_ZERO(hfo), 1.0f,
                               fmax(5.0f, sun.sun_intensity * 100.0f));
      sun_color[0] = sun_result.x * multiplier;
      sun_color[1] = sun_result.y * multiplier;
      sun_color[2] = sun_result.z * multiplier;
      float horizon_bias = fmin(0.5f, 
                                (fmax(0.0f, sun.direction[1] - 0.1f) * 10.0f));
      float max_component = fmax(sun_color[0], fmax(sun_color[1], sun_color[2]));
      sun_color[0] = mix(sun_color[0], max_component * 1.0f, horizon_bias);
      sun_color[1] = mix(sun_color[1], max_component * 0.98f, horizon_bias); 
      sun_color[2] = mix(sun_color[2], max_component * 0.73f, horizon_bias); 
      float ld_o[3];
      RELATIVE_TO_BASIS(sun_d, n, n_tangent, n_binormal, ld_o);
			float roughness[2] = { 1.0f - smoothness,
														 1.0f - smoothness };
      float ln_facet[3];
      ln_facet[0] = ld_i[0] + ld_o[0];
      ln_facet[1] = ld_i[1] + ld_o[1];
      ln_facet[2] = ld_i[2] + ld_o[2];
      NORMALIZE(ln_facet);
      float coating_dropoff = fmin(smoothness * 2.0f, 1.0f);
      float sample_dice;
      PRNG(global_id, b_seeds_2.x, sample_dice);
			if (glass_material || (sample_dice >= metallic)) {
        // Dielectric interface
        bool refract = (ld_i[2] * ld_o[2]) <= 0.0f;
        if (refract) {
          // Refraction facet
          float ior = (ld_i[2] < 0.0f) ? (1.0f / 1.5f) : 1.5f;
          ln_facet[0] = ld_i[0] + ld_o[0] * ior;
          ln_facet[1] = ld_i[1] + ld_o[1] * ior;
          ln_facet[2] = ld_i[2] + ld_o[2] * ior;
          NORMALIZE(ln_facet);
        }
        if (ln_facet[2] < 0.0f) {
          ln_facet[0] *= -1.0f;
          ln_facet[1] *= -1.0f;
          ln_facet[2] *= -1.0f;
        }
        float dot_ni = DOT(ld_i, ln_facet);
        float d, gi, go, g;
        GGX_EVAL(ln_facet, roughness, d);
        float cos_theta_t, fresnel;
        DIELECTRIC_FRESNEL(dot_ni, 1.5f, cos_theta_t, fresnel);
        SMITH_G1(ld_i, ln_facet, roughness, gi);
        SMITH_G1(ld_o, ln_facet, roughness, go);
        g = gi * go;
        if (!refract) {
          // Reflect/transmit to diffuse.
          float reflect_sample;
          PRNG(global_id, b_seeds_2.y, reflect_sample);
          if (glass_material || (reflect_sample <= (fresnel * coating_dropoff))) {
            // Reflection
            if ((last_brdf_pdf == DIRAC_PDF) && (smoothness < 1.0f)) {
              if ((d == 0.0f) || (ld_i[2] == 0.0f)) {
                sun_d[0] = FLT_MAX;
                brdf_pdf = DIRAC_PDF;
              } else {
                float model = fresnel * coating_dropoff * d * g / (4.0f * fabs(ld_i[2]));
                brdf_color[0] = model;
                brdf_color[1] = model;
                brdf_color[2] = model;
                float prob;
                prob = gi * fabs(dot_ni) * d / fabs(ld_i[2]);
                prob *= fresnel * coating_dropoff;
                float dwh_dwo = 1.0f / (4.0f * DOT(ld_o, ln_facet));
                brdf_pdf = fabs(prob * dwh_dwo);
              }
            } else {
              sun_d[0] = FLT_MAX;
              brdf_pdf = DIRAC_PDF;
            }
          } else {
            // Transmit to diffuse
            brdf_color[0] = albedo[0];
            brdf_color[1] = albedo[1];
            brdf_color[2] = albedo[2];
            brdf_pdf = INV_PI * ld_o[2];
          }
        } else {
          // Refraction
          if ((last_brdf_pdf == DIRAC_PDF) && (smoothness < 1.0f)) {
            if ((d == 0.0f) || (ld_i[2] == 0.0f)) {
              sun_d[0] = FLT_MAX;
              brdf_pdf = DIRAC_PDF;
            } else {
              float ior = (ld_i[2] < 0.0f) ? (1.0f / 1.5f) : 1.5f;
              float dot_no = DOT(ln_facet, ld_o);
              float sqrt_denom = dot_ni + ior * dot_no; 
              float model = ((1.0f - fresnel) * d * g * ior * ior * dot_ni * dot_no) /
                            (ld_i[2] * sqrt_denom * sqrt_denom);
              model *= (ld_i[2] < 0.0f) ? 1.5f : (1.0f / 1.5f);
              brdf_color[0] = albedo[0] * model;
              brdf_color[1] = albedo[1] * model;
              brdf_color[2] = albedo[2] * model;
              float prob;
              prob = gi * fabs(dot_ni) * d / fabs(ld_i[2]);
              prob *= fresnel * coating_dropoff;
              float dwh_dwo = (ior * ior * dot_no) / (sqrt_denom * sqrt_denom); 
              brdf_pdf = fabs(prob * dwh_dwo);
            }
          } else {
            sun_d[0] = FLT_MAX;
            brdf_pdf = DIRAC_PDF;
          }
        }
      } else {
        // Conductor
        if ((last_brdf_pdf == DIRAC_PDF) && (smoothness < 1.0f)) {
          float dot_ni = DOT(ln_facet, ld_i);
          float d, f, g, gi, go;
          GGX_EVAL(ln_facet, roughness, d);
          CONDUCTOR_FRESNEL(dot_ni, 1.5f, 3.0f, f);
          SMITH_G1(ld_i, ln_facet, roughness, gi);
          SMITH_G1(ld_o, ln_facet, roughness, go);
          g = gi * go;
          float model = f * (d * g / (4.0f * ld_i[2]));
          brdf_color[0] = albedo[0] * model;
          brdf_color[1] = albedo[1] * model;
          brdf_color[2] = albedo[2] * model;
          brdf_pdf = d * gi / (4.0f * ld_i[2]);
        } else {
          sun_d[0] = FLT_MAX;
          brdf_pdf = DIRAC_PDF;
        }
      }
      float mis = MIS(sun_pdf, brdf_pdf);
      ray_state[RS_SUN_R] = old_throughput[0] * (sun_color[0] / NOT_ZERO(sun_pdf)) *
                            brdf_color[0] * mis;
      ray_state[RS_SUN_G] = old_throughput[1] * (sun_color[1] / NOT_ZERO(sun_pdf)) *
                            brdf_color[1] * mis;
      ray_state[RS_SUN_B] = old_throughput[2] * (sun_color[2] / NOT_ZERO(sun_pdf)) *
                            brdf_color[2] * mis;
    }
  }
  float o[3];
  o[0] = ray_state[RS_ORIGIN_X];
  o[1] = ray_state[RS_ORIGIN_Y];
  o[2] = ray_state[RS_ORIGIN_Z];
  float emitter_d[3];
  emitter_d[0] = FLT_MAX;
  if (emitter_count > 0) {
    float emitter_uv_sample[2];
    PRNG(global_id, b_seeds_2.z, emitter_uv_sample[0]);
    PRNG(global_id, b_seeds_2.w, emitter_uv_sample[1]);
    if ((emitter_uv_sample[0] + emitter_uv_sample[1]) >= 1.0f) {
      emitter_uv_sample[0] = 1.0f - emitter_uv_sample[0]; 
      emitter_uv_sample[1] = 1.0f - emitter_uv_sample[1]; 
    }
    float emit_u = emitter_uv_sample[0];
    float emit_v = emitter_uv_sample[1];
    float emit_w = 1.0f - emit_u - emit_v;
    float emitter_sample;
    PRNG(global_id, b_seeds_3.x, emitter_sample);
    uint emitter_choice = emitter_sample * emitter_count;
    EmitterIndex emitter_index = emitters[emitter_choice];
    uint emitter_tri_index = emitter_index.mesh_triangle_offset + 
                                 emitter_index.triangle_offset;
    int tri_pixel = emitter_tri_index * 42;
    float emitter_position[3];
    if (use_triangles_image) {
      emitter_position[0] = read_imagef(triangles_image, TRI_VERTEX).x +
        (emit_u * read_imagef(triangles_image, TRI_EDGES_0).x) +
        (emit_v * -read_imagef(triangles_image, TRI_EDGES_1).x);
      emitter_position[1] = read_imagef(triangles_image, TRI_VERTEX + 1).x +
        (emit_u * read_imagef(triangles_image, TRI_EDGES_0 + 1).x) +
        (emit_v * -read_imagef(triangles_image, TRI_EDGES_1 + 1).x);
      emitter_position[2] = read_imagef(triangles_image, TRI_VERTEX + 2).x +
        (emit_u * read_imagef(triangles_image, TRI_EDGES_0 + 2).x) +
        (emit_v * -read_imagef(triangles_image, TRI_EDGES_1 + 2).x);
    } else {
      emitter_position[0] = triangles[emitter_tri_index].vertex[0] +
        (emit_u * triangles[emitter_tri_index].edges[0][0]) +
        (emit_v * -triangles[emitter_tri_index].edges[1][0]);
      emitter_position[1] = triangles[emitter_tri_index].vertex[1] +
        (emit_u * triangles[emitter_tri_index].edges[0][1]) +
        (emit_v * -triangles[emitter_tri_index].edges[1][1]);
      emitter_position[2] = triangles[emitter_tri_index].vertex[2] +
        (emit_u * triangles[emitter_tri_index].edges[0][2]) +
        (emit_v * -triangles[emitter_tri_index].edges[1][2]);
    }
    emitter_d[0] = emitter_position[0] - o[0];
    emitter_d[1] = emitter_position[1] - o[1];
    emitter_d[2] = emitter_position[2] - o[2];
    float t_2 = DOT(emitter_d, emitter_d);
    float t = native_sqrt(t_2);
    ray_state[RS_EMITTER_T] = t;
    float inv_t = 1.0f / t;
    emitter_d[0] *= inv_t;
    emitter_d[1] *= inv_t;
    emitter_d[2] *= inv_t;
    float emitter_n[3];
    float inverse_area;
    uint mat_id;
    if (use_triangles_image) {
      mat_id = read_imageui(triangles_image, TRI_MAT_ID).x;
      inverse_area = read_imagef(triangles_image, TRI_INV_AREA).x;
      emitter_n[0] = emit_w * read_imagef(triangles_image, TRI_VN_0).x + 
                     emit_u * read_imagef(triangles_image, TRI_VN_1).x +  
                     emit_v * read_imagef(triangles_image, TRI_VN_2).x;
      emitter_n[1] = emit_w * read_imagef(triangles_image, TRI_VN_0 + 1).x + 
                     emit_u * read_imagef(triangles_image, TRI_VN_1 + 1).x +  
                     emit_v * read_imagef(triangles_image, TRI_VN_2 + 1).x;
      emitter_n[2] = emit_w * read_imagef(triangles_image, TRI_VN_0 + 2).x + 
                     emit_u * read_imagef(triangles_image, TRI_VN_1 + 2).x +  
                     emit_v * read_imagef(triangles_image, TRI_VN_2 + 2).x;
    } else {
      mat_id = triangles[emitter_tri_index].material_id; 
      inverse_area = triangles[emitter_tri_index].inverse_area; 
      emitter_n[0] = emit_w * triangles[emitter_tri_index].vertex_normals[0][0] + 
                     emit_u * triangles[emitter_tri_index].vertex_normals[1][0] + 
                     emit_v * triangles[emitter_tri_index].vertex_normals[2][0];
      emitter_n[1] = emit_w * triangles[emitter_tri_index].vertex_normals[0][1] + 
                     emit_u * triangles[emitter_tri_index].vertex_normals[1][1] + 
                     emit_v * triangles[emitter_tri_index].vertex_normals[2][1];
      emitter_n[2] = emit_w * triangles[emitter_tri_index].vertex_normals[0][2] + 
                     emit_u * triangles[emitter_tri_index].vertex_normals[1][2] + 
                     emit_v * triangles[emitter_tri_index].vertex_normals[2][2];
    }
    NORMALIZE(emitter_n);
    float cos_at_emitter = DOT(-emitter_d, emitter_n);
    float cos_at_tri = DOT(emitter_d, n);
    if ((cos_at_emitter <= 0.0f) || (cos_at_tri <= 0.0f) || 
        (t_2 < (HI_EPSILON * 4.0f))) {
      emitter_d[0] = FLT_MAX; 
    } else {
      float brdf_color[3];
      float brdf_pdf;
      float emitter_prob = 1.0f / (float)emitter_count;
      float emitter_pdf = inverse_area * (t_2 / fabs(cos_at_emitter)) * emitter_prob;
      float intensity = materials[mat_id].emission * 25.0f;
      float emitter_color[3];
      float uv[2];
      if (materials[mat_id].map_flags & MAP_FLAG_EMISSION) {
        float2 emission_uv;
        if (use_triangles_image) {
          emission_uv.x = emit_w * read_imagef(triangles_image, TRI_UV_0).x +
                          emit_u * read_imagef(triangles_image, TRI_UV_1).x +
                          emit_v * read_imagef(triangles_image, TRI_UV_2).x;
          emission_uv.y = emit_w * read_imagef(triangles_image, TRI_UV_0 + 1).x +
                          emit_u * read_imagef(triangles_image, TRI_UV_1 + 1).x +
                          emit_v * read_imagef(triangles_image, TRI_UV_2 + 1).x;
        } else {
          emission_uv.x = emit_w * triangles[emitter_tri_index].vertex_uvs[0][0] +
                          emit_u * triangles[emitter_tri_index].vertex_uvs[1][0] +
                          emit_v * triangles[emitter_tri_index].vertex_uvs[2][0];
          emission_uv.y = emit_w * triangles[emitter_tri_index].vertex_uvs[0][1] +
                          emit_u * triangles[emitter_tri_index].vertex_uvs[1][1] +
                          emit_v * triangles[emitter_tri_index].vertex_uvs[2][1];
        }
        emission_uv.x = mix(materials[mat_id].emission_bounds[0] + 0.5f,
                             materials[mat_id].emission_bounds[1] - 0.5f, 
                             emission_uv.x);
        emission_uv.y = mix(materials[mat_id].emission_bounds[2] + 0.5f,
                             materials[mat_id].emission_bounds[3] - 0.5f, 
                             emission_uv.y);
        const sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE |
                                  CLK_ADDRESS_REPEAT | CLK_FILTER_LINEAR;
        float4 emission_map = read_imagef(emission_atlas, sampler, emission_uv);
        emitter_color[0] = emission_map.x * intensity;
        emitter_color[1] = emission_map.y * intensity;
        emitter_color[2] = emission_map.z * intensity;
      } else {
        emitter_color[0] = materials[mat_id].albedo[0] * intensity;
        emitter_color[1] = materials[mat_id].albedo[1] * intensity;
        emitter_color[2] = materials[mat_id].albedo[2] * intensity;
      }
      if (fmax(fmax(emitter_color[0], emitter_color[1]), emitter_color[2]) > 0.0f) {
        emitter_color[0] /= emitter_pdf;
        emitter_color[1] /= emitter_pdf;
        emitter_color[2] /= emitter_pdf;
        float ld_o[3];
        RELATIVE_TO_BASIS(emitter_d, n, n_tangent, n_binormal, ld_o);
        float roughness[2] = { 1.0f - smoothness,
                               1.0f - smoothness };
        float ln_facet[3];
        ln_facet[0] = ld_i[0] + ld_o[0];
        ln_facet[1] = ld_i[1] + ld_o[1];
        ln_facet[2] = ld_i[2] + ld_o[2];
        NORMALIZE(ln_facet);
        float coating_dropoff = fmin(smoothness * 2.0f, 1.0f);
        float sample_dice;
        PRNG(global_id, b_seeds_2.x, sample_dice);
        if (glass_material || (sample_dice >= metallic)) {
          // Dielectric interface
          bool refract = (ld_i[2] * ld_o[2]) <= 0.0f;
          if (refract) {
            // Refraction facet
            float ior = (ld_i[2] < 0.0f) ? (1.0f / 1.5f) : 1.5f;
            ln_facet[0] = ld_i[0] + ld_o[0] * ior;
            ln_facet[1] = ld_i[1] + ld_o[1] * ior;
            ln_facet[2] = ld_i[2] + ld_o[2] * ior;
            NORMALIZE(ln_facet);
          }
          if (ln_facet[2] < 0.0f) {
            ln_facet[0] *= -1.0f;
            ln_facet[1] *= -1.0f;
            ln_facet[2] *= -1.0f;
          }
          float dot_ni = DOT(ld_i, ln_facet);
          float d, gi, go, g;
          GGX_EVAL(ln_facet, roughness, d);
          float cos_theta_t, fresnel;
          DIELECTRIC_FRESNEL(dot_ni, 1.5f, cos_theta_t, fresnel);
          SMITH_G1(ld_i, ln_facet, roughness, gi);
          SMITH_G1(ld_o, ln_facet, roughness, go);
          g = gi * go;
          if (!refract) {
            // Reflect/transmit to diffuse.
            float reflect_sample;
            PRNG(global_id, b_seeds_2.y, reflect_sample);
            if (glass_material || (reflect_sample <= (fresnel * coating_dropoff))) {
              // Reflection
              if ((last_brdf_pdf == DIRAC_PDF) && (smoothness < 1.0f)) {
                if ((d == 0.0f) || (ld_i[2] == 0.0f)) {
                  emitter_d[0] = FLT_MAX;
                  brdf_pdf = DIRAC_PDF;
                } else {
                  float model = fresnel * coating_dropoff * d * g / (4.0f * fabs(ld_i[2]));
                  brdf_color[0] = model;
                  brdf_color[1] = model;
                  brdf_color[2] = model;
                  float prob;
                  prob = gi * fabs(dot_ni) * d / fabs(ld_i[2]);
                  prob *= fresnel * coating_dropoff;
                  float dwh_dwo = 1.0f / (4.0f * DOT(ld_o, ln_facet));
                  brdf_pdf = fabs(prob * dwh_dwo);
                }
              } else {
                emitter_d[0] = FLT_MAX;
              }
            } else {
              // Transmit to diffuse
              brdf_color[0] = albedo[0];
              brdf_color[1] = albedo[1];
              brdf_color[2] = albedo[2];
              brdf_pdf = INV_PI * ld_o[2];
            }
          } else {
            // Refraction
            if ((last_brdf_pdf == DIRAC_PDF) && (smoothness < 1.0f)) {
              if ((d == 0.0f) || (ld_i[2] == 0.0f)) {
                emitter_d[0] = FLT_MAX;
                brdf_pdf = DIRAC_PDF;
              } else {
                float ior = (ld_i[2] < 0.0f) ? (1.0f / 1.5f) : 1.5f;
                float dot_no = DOT(ln_facet, ld_o);
                float sqrt_denom = dot_ni + ior * dot_no; 
                float model = ((1.0f - fresnel) * d * g * ior * ior * dot_ni * dot_no) /
                              (ld_i[2] * sqrt_denom * sqrt_denom);
                model *= (ld_i[2] < 0.0f) ? 1.5f : (1.0f / 1.5f);
                brdf_color[0] = albedo[0] * model;
                brdf_color[1] = albedo[1] * model;
                brdf_color[2] = albedo[2] * model;
                float prob;
                prob = gi * fabs(dot_ni) * d / fabs(ld_i[2]);
                prob *= fresnel * coating_dropoff;
                float dwh_dwo = (ior * ior * dot_no) / (sqrt_denom * sqrt_denom); 
                brdf_pdf = fabs(prob * dwh_dwo);
              }
            } else {
              emitter_d[0] = FLT_MAX;
            }
          }
        } else {
          // Conductor
          if ((last_brdf_pdf == DIRAC_PDF) && (smoothness < 1.0f)) {
            float dot_ni = DOT(ln_facet, ld_i);
            float d, f, g, gi, go;
            GGX_EVAL(ln_facet, roughness, d);
            CONDUCTOR_FRESNEL(dot_ni, 1.5f, 3.0f, f);
            SMITH_G1(ld_i, ln_facet, roughness, gi);
            SMITH_G1(ld_o, ln_facet, roughness, go);
            g = gi * go;
            float model = f * (d * g / (4.0f * ld_i[2]));
            brdf_color[0] = albedo[0] * model;
            brdf_color[1] = albedo[1] * model;
            brdf_color[2] = albedo[2] * model;
            brdf_pdf = d * gi / (4.0f * ld_i[2]);
          } else {
            emitter_d[0] = FLT_MAX;
          }
        }
      } else {
        emitter_d[0] = FLT_MAX;
      }
      float mis = MIS(emitter_pdf, brdf_pdf);
      ray_state[RS_EMITTER_R] = old_throughput[0] * emitter_color[0] * 
                                brdf_color[0] * mis;
      ray_state[RS_EMITTER_G] = old_throughput[1] * emitter_color[1] * 
                                brdf_color[1] * mis;
      ray_state[RS_EMITTER_B] = old_throughput[2] * emitter_color[2] * 
                                brdf_color[2] * mis;
    }
  }
  ray_state[RS_SUN_D_X] = sun_d[0];
  ray_state[RS_SUN_D_Y] = sun_d[1];
  ray_state[RS_SUN_D_Z] = sun_d[2];
  ray_state[RS_EMITTER_D_X] = emitter_d[0];
  ray_state[RS_EMITTER_D_Y] = emitter_d[1];
  ray_state[RS_EMITTER_D_Z] = emitter_d[2];
}
///
kernel void NEE(global float* ray_state, 
                global Material* materials,
                int use_nodes_image,
                read_only image1d_buffer_t nodes_image,
                global TwoNode* nodes,
                int top_tree_offset,
                int use_triangles_image,
                read_only image1d_buffer_t triangles_image,
                global TreeTriangle* triangles,
                read_only image2d_t albedo_atlas,
                read_only image2d_t emission_atlas,
                uint4  b_seeds,
                int width, 
                int height)
{
  int global_id = get_global_id(0); 
  int ray_count = width * height;
  if (global_id >= ray_count) {
    return;
  }
  int state = ray_state[RS_STATE];
  if (state == STATE_END) {
    return;
  }
  float o[3];
  o[0] = ray_state[RS_ORIGIN_X];
  o[1] = ray_state[RS_ORIGIN_Y];
  o[2] = ray_state[RS_ORIGIN_Z];
  float sun_d[3];
  sun_d[0] = ray_state[RS_SUN_D_X];
  if (sun_d[0] < FLT_MAX) {
    sun_d[1] = ray_state[RS_SUN_D_Y];
    sun_d[2] = ray_state[RS_SUN_D_Z];
    float sun_o[3];
    sun_o[0] = o[0] + (sun_d[0] * EPSILON);
    sun_o[1] = o[1] + (sun_d[1] * EPSILON);
    sun_o[2] = o[2] + (sun_d[2] * EPSILON);
    float h_t = FLT_MAX;
    OCCLUSION(sun_o, sun_d, use_nodes_image, use_triangles_image);
    if (h_t == FLT_MAX) {
      ray_state[RS_COLOR_R] += ray_state[RS_SUN_R];
      ray_state[RS_COLOR_G] += ray_state[RS_SUN_G];
      ray_state[RS_COLOR_B] += ray_state[RS_SUN_B];
    }  
  }
  float emitter_d[3];
  emitter_d[0] = ray_state[RS_EMITTER_D_X];
  if (emitter_d[0] < FLT_MAX) {
    emitter_d[1] = ray_state[RS_EMITTER_D_Y];
    emitter_d[2] = ray_state[RS_EMITTER_D_Z];
    float emitter_o[3];
    emitter_o[0] = o[0] + (emitter_d[0] * EPSILON);
    emitter_o[1] = o[1] + (emitter_d[1] * EPSILON);
    emitter_o[2] = o[2] + (emitter_d[2] * EPSILON);
    float emitter_t = ray_state[RS_EMITTER_T] - (2.0f * EPSILON);
    float h_t = emitter_t;
    OCCLUSION(emitter_o, emitter_d, use_nodes_image, use_triangles_image);
    if (h_t == emitter_t) {
      ray_state[RS_COLOR_R] += ray_state[RS_EMITTER_R];
      ray_state[RS_COLOR_G] += ray_state[RS_EMITTER_G];
      ray_state[RS_COLOR_B] += ray_state[RS_EMITTER_B];
    }  
  }
  float throughput[3];
  throughput[0] = ray_state[RS_THROUGHPUT_R];
  throughput[1] = ray_state[RS_THROUGHPUT_G];
  throughput[2] = ray_state[RS_THROUGHPUT_B];
  float tp = fmax(fmax(throughput[0], throughput[1]), throughput[2]);
  if (tp <= THROUGHPUT_THRESHOLD) {
    ray_state[RS_STATE] = STATE_END;
  } else {
    float roulette_prob = tp;
    if (roulette_prob <= 1.0f) {
      float roulette;
      PRNG(global_id, b_seeds.z, roulette);
      if (roulette < roulette_prob) {
        float modify_throughput = 1.0f / roulette_prob;
        ray_state[RS_THROUGHPUT_R] = throughput[0] * modify_throughput;
        ray_state[RS_THROUGHPUT_G] = throughput[1] * modify_throughput;
        ray_state[RS_THROUGHPUT_B] = throughput[2] * modify_throughput;
      } else {
        ray_state[RS_STATE] = STATE_END;
      }
    }
  }
}
///
kernel void Accumulate(global float* ray_state,
                       write_only image2d_t target,
                       ImageProperties image_properties,
                       int width,
                       int height,
                       float inv_sample_count)
{
  int global_id = get_global_id(1) * get_global_size(0) + get_global_id(0);
  int ray_count = width * height;
  if (global_id >= ray_count) {
    return;
  }
  float rgb[3];
	rgb[0] = ray_state[RS_COLOR_R];
	rgb[1] = ray_state[RS_COLOR_G];
	rgb[2] = ray_state[RS_COLOR_B];
	AVG_TONEMAP(rgb);
  float lum_coeff[3];
	lum_coeff[0] = 0.2125f;
	lum_coeff[1] = 0.7154f;
	lum_coeff[2] = 0.0721f;
	float bright_rgb[3];
	bright_rgb[0] = rgb[0] * image_properties.brightness;
	bright_rgb[1] = rgb[1] * image_properties.brightness;
	bright_rgb[2] = rgb[2] * image_properties.brightness;
	float intensity = DOT(bright_rgb, lum_coeff);
	float sat_color[3];
	sat_color[0] = mix(intensity, bright_rgb[0], image_properties.saturation);
	sat_color[1] = mix(intensity, bright_rgb[1], image_properties.saturation);
	sat_color[2] = mix(intensity, bright_rgb[2], image_properties.saturation);
  rgb[0] = mix(0.5f, sat_color[0], image_properties.contrast);
	rgb[1] = mix(0.5f, sat_color[1], image_properties.contrast);
	rgb[2] = mix(0.5f, sat_color[2], image_properties.contrast);
  float4 target_color = (float4)(rgb[0], rgb[1], rgb[2], 1.0f);
  target_color = clamp(target_color, 0.0f, 1.0f);
  int2 xy = (int2)(global_id % width, global_id / width);
  write_imagef(target, xy, target_color);
}
///
kernel void CompositeTwo(read_only image2d_t compositing_target_one,
                         read_only image2d_t compositing_target_two,
                         write_only image2d_t target,
                         int width,
                         int height)
{
  int global_id = get_global_id(1) * get_global_size(0) + get_global_id(0);
  int ray_count = width * height;
  if (global_id >= ray_count) {
    return;
  }
  const sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE |
                            CLK_ADDRESS_CLAMP | CLK_FILTER_NEAREST;
  int2 xy = (int2)(global_id % width, global_id / width);
  float4 c0 = read_imagef(compositing_target_one, sampler, xy);
  float4 c1 = read_imagef(compositing_target_two, sampler, xy);
  float4 target_color = (c0 + c1) * 0.5f;
  write_imagef(target, xy, target_color);
}
///
kernel void CompositeThree(read_only image2d_t compositing_target_one,
                           read_only image2d_t compositing_target_two,
                           read_only image2d_t compositing_target_three,
                           write_only image2d_t target,
                           int width,
                           int height)
{
  int global_id = get_global_id(1) * get_global_size(0) + get_global_id(0);
  int ray_count = width * height;
  if (global_id >= ray_count) {
    return;
  }
  int2 xy = (int2)(global_id % width, global_id / width);
  const sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE |
                            CLK_ADDRESS_CLAMP | CLK_FILTER_NEAREST;
  float4 c0 = read_imagef(compositing_target_one, sampler, xy);
  float4 c1 = read_imagef(compositing_target_two, sampler, xy);
  float4 c2 = read_imagef(compositing_target_three, sampler, xy);
  float4 target_color = (c0 + c1 + c2) * 0.3333333f;
  write_imagef(target, xy, target_color);
}
///
kernel void CompositeFour(read_only image2d_t compositing_target_one,
                          read_only image2d_t compositing_target_two,
                          read_only image2d_t compositing_target_three,
                          read_only image2d_t compositing_target_four,
                          write_only image2d_t target,
                          int width,
                          int height)
{
  int global_id = get_global_id(1) * get_global_size(0) + get_global_id(0);
  int ray_count = width * height;
  if (global_id >= ray_count) {
    return;
  }
  int2 xy = (int2)(global_id % width, global_id / width);
  const sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE |
                            CLK_ADDRESS_CLAMP | CLK_FILTER_NEAREST;
  float4 c0 = read_imagef(compositing_target_one, sampler, xy);
  float4 c1 = read_imagef(compositing_target_two, sampler, xy);
  float4 c2 = read_imagef(compositing_target_three, sampler, xy);
  float4 c3 = read_imagef(compositing_target_four, sampler, xy);
  float4 target_color = (c0 + c1 + c2 + c3) * 0.25f;
  write_imagef(target, xy, target_color);
}
///
kernel void CompositeFive(read_only image2d_t compositing_target_one,
                          read_only image2d_t compositing_target_two,
                          read_only image2d_t compositing_target_three,
                          read_only image2d_t compositing_target_four,
                          read_only image2d_t compositing_target_five,
                          write_only image2d_t target,
                          int width,
                          int height)
{
  int global_id = get_global_id(1) * get_global_size(0) + get_global_id(0);
  int ray_count = width * height;
  if (global_id >= ray_count) {
    return;
  }
  int2 xy = (int2)(global_id % width, global_id / width);
  const sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE |
                            CLK_ADDRESS_CLAMP | CLK_FILTER_NEAREST;
  float4 c0 = read_imagef(compositing_target_one, sampler, xy);
  float4 c1 = read_imagef(compositing_target_two, sampler, xy);
  float4 c2 = read_imagef(compositing_target_three, sampler, xy);
  float4 c3 = read_imagef(compositing_target_four, sampler, xy);
  float4 c4 = read_imagef(compositing_target_five, sampler, xy);
  float4 target_color = (c0 + c1 + c2 + c3 + c4) * 0.2f;
  write_imagef(target, xy, target_color);
}
