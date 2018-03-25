std::string program = "typedef struct TreeTriangle\n\
{\n\
  float vertex[3];\n\
  float edges[2][3];\n\
  float vertex_normals[3][3];\n\
  float vertex_tangents[3][4];\n\
  float vertex_uvs[3][2];\n\
  float surface_normal[3];\n\
  float inverse_area;\n\
  uint material_id;\n\
  uint pad;\n\
} TreeTriangle;\n\
///\n\
#define TRI_VERTEX   (tri_pixel)\n\
#define TRI_EDGES_0  (tri_pixel + 3)\n\
#define TRI_EDGES_1  (tri_pixel + 6)\n\
#define TRI_VN_0     (tri_pixel + 9)\n\
#define TRI_VN_1     (tri_pixel + 12)\n\
#define TRI_VN_2     (tri_pixel + 15)\n\
#define TRI_VT_0     (tri_pixel + 18)\n\
#define TRI_VT_1     (tri_pixel + 22)\n\
#define TRI_VT_2     (tri_pixel + 26)\n\
#define TRI_UV_0     (tri_pixel + 30)\n\
#define TRI_UV_1     (tri_pixel + 32)\n\
#define TRI_UV_2     (tri_pixel + 34)\n\
#define TRI_SURF_N   (tri_pixel + 36)\n\
#define TRI_INV_AREA (tri_pixel + 39)\n\
#define TRI_MAT_ID   (tri_pixel + 40)\n\
///\n\
/*\n\
typedef struct FourNode\n\
{\n\
  uint child[4];\n\
  uint extra[2];\n\
  float ll_bounds[6];\n\
  float lr_bounds[6];\n\
  float rl_bounds[6];\n\
  float rr_bounds[6];\n\
  float padding[2];\n\
} FourNode;\n\
///\n\
#define NODE_LL        (node_pixel + 0)\n\
#define NODE_LR        (node_pixel + 1)\n\
#define NODE_RL        (node_pixel + 2)\n\
#define NODE_RR        (node_pixel + 3)\n\
#define NODE_EXTRA_0   (node_pixel + 4)\n\
#define NODE_EXTRA_1   (node_pixel + 5)\n\
#define NODE_LL_BOUNDS (node_pixel + 6)\n\
#define NODE_LR_BOUNDS (node_pixel + 12)\n\
#define NODE_RL_BOUNDS (node_pixel + 18)\n\
#define NODE_RR_BOUNDS (node_pixel + 24)\n\
*/\n\
///\n\
typedef struct TwoNode\n\
{\n\
  uint child[2];\n\
  uint extra[2];\n\
  float l_bounds[6];\n\
  float r_bounds[6];\n\
} TwoNode;\n\
///\n\
#define NODE_L        (node_pixel + 0)\n\
#define NODE_R        (node_pixel + 1)\n\
#define NODE_EXTRA_0   (node_pixel + 2)\n\
#define NODE_EXTRA_1   (node_pixel + 3)\n\
#define NODE_L_BOUNDS (node_pixel + 4)\n\
#define NODE_R_BOUNDS (node_pixel + 10)\n\
///\n\
typedef struct Camera\n\
{\n\
  float aperture_size;\n\
  float focal_depth;\n\
  float transform[16];\n\
  float root_pixel[2];\n\
  float pixel_offset[2];\n\
  float image_plane_distance;\n\
  float padding;\n\
} Camera;\n\
///\n\
typedef struct Material \n\
{\n\
  uint map_flags;\n\
  int double_sided;\n\
  float albedo[4];\n\
  float metallic;\n\
  float smoothness;\n\
  float glass;\n\
  float emission;\n\
  float albedo_bounds[4];\n\
  float normal_bounds[4];\n\
  float metallic_bounds[4];\n\
  float emission_bounds[4];\n\
} Material;\n\
///\n\
typedef struct Sun \n\
{\n\
  float direction[4];\n\
  float sun_intensity;\n\
  float sky_intensity;\n\
  float color[4];\n\
} Sun;\n\
///\n\
typedef struct EnvironmentProperties\n\
{\n\
  float intensity;\n\
  float rotation;\n\
} EnvironmentProperties;\n\
///\n\
typedef struct EmitterIndex \n\
{\n\
  uint mesh_triangle_offset;\n\
  uint triangle_offset;\n\
} EmitterIndex;\n\
///\n\
typedef struct PortalIndex \n\
{\n\
  uint mesh_triangle_offset;\n\
  uint triangle_offset;\n\
} PortalIndex;\n\
///\n\
typedef struct ImageProperties {\n\
  int tonemap;\n\
  float gamma;\n\
  float exposure;\n\
  float contrast;\n\
  float saturation;\n\
  float brightness;\n\
} ImageProperties;\n\
///\n\
#define TREE_LEAF_TRIANGLE_FACTOR 3\n\
///\n\
#define HM_TEST_NODES 0x0\n\
#define HM_LEFT       0x2\n\
#define HM_RIGHT      0x1\n\
#define HM_BOTH       0x3\n\
///\n\
#define LB_NEW_SUB_TREE 0xFFFFFFFF\n\
///\n\
#define TREE_LEAF            0x80000000\n\
#define TREE_LEAF_MASK       0x7FFFFFFF\n\
#define TREE_SUB_TREE        0x40000000\n\
#define TREE_SUB_TREE_MASK   0x3FFFFFFF\n\
#define TREE_TRAIL_END       0x7FFFFFFF\n\
#define TREE_NO_LEAF_HIT     0xFFFFFFFF\n\
#define NO_TRI_HIT           INT_MAX\n\
///\n\
#define DOUBLE_SIDED_FLAG 0x80000000\n\
#define DOUBLE_SIDED_MASK 0x7FFFFFFF\n\
///\n\
#define THROUGHPUT_THRESHOLD 0.05f\n\
///\n\
#define DIRAC_PDF FLT_MAX\n\
///\n\
#define MAP_FLAG_ALBEDO   0x1\n\
#define MAP_FLAG_NORMAL   0x2\n\
#define MAP_FLAG_METALLIC 0x4\n\
#define MAP_FLAG_EMISSION 0x8\n\
///\n\
#define SUN_COS_THETA 0.9996f\n\
///\n\
#define STATE_ENVIRONMENT 0x0\n\
#define STATE_EMISSIVE 0x1\n\
#define STATE_MATERIAL 0x2\n\
#define STATE_SECONDARY 0x3\n\
#define STATE_END 0x4\n\
///\n\
#define RS_STATE         (ray_count * 0 + global_id)\n\
#define RS_ORIGIN_X      (ray_count * 1 + global_id)\n\
#define RS_ORIGIN_Y      (ray_count * 2 + global_id)\n\
#define RS_ORIGIN_Z      (ray_count * 3 + global_id)\n\
#define RS_DIR_X         (ray_count * 4 + global_id)\n\
#define RS_DIR_Y         (ray_count * 5 + global_id)\n\
#define RS_DIR_Z         (ray_count * 6 + global_id)\n\
#define RS_U             (ray_count * 7 + global_id)\n\
#define RS_V             (ray_count * 8 + global_id)\n\
#define RS_HIT_DISTANCE  (ray_count * 9 + global_id)\n\
#define RS_TRI_INDEX     (ray_count * 10 + global_id)\n\
#define RS_LAST_BRDF_PDF (ray_count * 11 + global_id)\n\
#define RS_THROUGHPUT_R  (ray_count * 12 + global_id)\n\
#define RS_THROUGHPUT_G  (ray_count * 13 + global_id)\n\
#define RS_THROUGHPUT_B  (ray_count * 14 + global_id)\n\
#define RS_COLOR_R       (ray_count * 15 + global_id)\n\
#define RS_COLOR_G       (ray_count * 16 + global_id)\n\
#define RS_COLOR_B       (ray_count * 17 + global_id)\n\
#define RS_SUN_D_X       (ray_count * 18 + global_id)\n\
#define RS_SUN_D_Y       (ray_count * 19 + global_id)\n\
#define RS_SUN_D_Z       (ray_count * 20 + global_id)\n\
#define RS_SUN_R         (ray_count * 21 + global_id)\n\
#define RS_SUN_G         (ray_count * 22 + global_id)\n\
#define RS_SUN_B         (ray_count * 23 + global_id)\n\
#define RS_EMITTER_D_X     (ray_count * 24 + global_id)\n\
#define RS_EMITTER_D_Y     (ray_count * 25 + global_id)\n\
#define RS_EMITTER_D_Z     (ray_count * 26 + global_id)\n\
#define RS_EMITTER_T       (ray_count * 27 + global_id)\n\
#define RS_EMITTER_R       (ray_count * 28 + global_id)\n\
#define RS_EMITTER_G       (ray_count * 29 + global_id)\n\
#define RS_EMITTER_B       (ray_count * 30 + global_id)\n\
#define RS_PORTAL_D_X     (ray_count * 31 + global_id)\n\
#define RS_PORTAL_D_Y     (ray_count * 32 + global_id)\n\
#define RS_PORTAL_D_Z     (ray_count * 33 + global_id)\n\
#define RS_PORTAL_R       (ray_count * 34 + global_id)\n\
#define RS_PORTAL_G       (ray_count * 35 + global_id)\n\
#define RS_PORTAL_B       (ray_count * 36 + global_id)\n\
///\n\
#define HI_EPSILON 0.01f\n\
#define EPSILON 0.0001f\n\
#define LO_EPSILON 0.000001f\n\
#define PI 3.14159265359f\n\
#define INV_PI 0.31830988618f\n\
#define TAU 6.28318530718f\n\
#define INV_TAU 0.15915494309\n\
///\n\
#define PRNG(P, SEED, R) \\\n\
{ \\\n\
  uint hash = SEED; \\\n\
  hash ^= P; \\\n\
  hash += (hash << 10); \\\n\
  hash ^= (hash >>  6); \\\n\
  hash += (hash <<  3); \\\n\
  hash ^= (hash >> 11); \\\n\
  hash += (hash << 15); \\\n\
  hash ^= hash >> 16; \\\n\
  hash *= 0x85EBCA6B; \\\n\
  hash ^= hash >> 13; \\\n\
  hash *= 0xC2B2AE35; \\\n\
  hash ^= hash >> 16; \\\n\
  hash &= 0x007FFFFFu; \\\n\
  hash |= 0x3F800000u; \\\n\
  R = *((float*)&hash) - 1.0f; \\\n\
	R = fmax(EPSILON, R); \\\n\
}\n\
///\n\
#define TRANSFORM_X(P, MAT, R) { \\\n\
  R[0] = P[0] * MAT[0] + P[1] * MAT[4] + P[2] * MAT[8] + MAT[12]; \\\n\
}\n\
#define TRANSFORM_Y(P, MAT, R) { \\\n\
  R[1] = P[0] * MAT[1] + P[1] * MAT[5] + P[2] * MAT[9] + MAT[13]; \\\n\
}\n\
#define TRANSFORM_Z(P, MAT, R) { \\\n\
  R[2] = P[0] * MAT[2] + P[1] * MAT[6] + P[2] * MAT[10] + MAT[14]; \\\n\
}\n\
#define TRANSFORM_W(P, MAT, R) { \\\n\
  R[3] = P[0] * MAT[3] + P[1] * MAT[7] + P[2] * MAT[11] + MAT[15]; \\\n\
}\n\
///\n\
#define DOT(A, B) (A[0] * B[0] + A[1] * B[1] + A[2] * B[2])\n\
///\n\
#define CROSS(A, B, C) { \\\n\
  C[0] = A[1] * B[2] - A[2] * B[1]; \\\n\
  C[1] = A[2] * B[0] - A[0] * B[2]; \\\n\
  C[2] = A[0] * B[1] - A[1] * B[0]; \\\n\
}\n\
///\n\
#define NOT_ZERO(A) copysign(fmax(LO_EPSILON, fabs(A)), (A))\n\
///\n\
#define NORMALIZE(V) \\\n\
{ \\\n\
  float length = native_sqrt(V[0] * V[0] + V[1] * V[1] + V[2] * V[2]); \\\n\
  float inv_length = 1.0f / NOT_ZERO(length); \\\n\
  V[0] *= inv_length; \\\n\
  V[1] *= inv_length; \\\n\
  V[2] *= inv_length; \\\n\
}\n\
///\n\
#define GGX_SAMPLE(D_I, ROUGHNESS, RXF, RYF, N) \\\n\
{ \\\n\
  float stretched[3]; \\\n\
  stretched[0] = ROUGHNESS[0] * D_I[0]; \\\n\
  stretched[1] = ROUGHNESS[1] * D_I[1]; \\\n\
  stretched[2] = D_I[2]; \\\n\
  NORMALIZE(stretched); \\\n\
  float up[3]; \\\n\
  up[0] = 0.0f; \\\n\
  up[1] = 0.0f; \\\n\
  up[2] = 1.0f; \\\n\
  float t1[3]; \\\n\
  CROSS(stretched, up, t1); \\\n\
  if (stretched[2] < 0.9999f) { \\\n\
    NORMALIZE(t1); \\\n\
  } else { \\\n\
    t1[0] = 1.0f; \\\n\
    t1[1] = 0.0f; \\\n\
    t1[2] = 0.0f; \\\n\
  } \\\n\
  float t2[3]; \\\n\
  CROSS(t1, stretched, t2); \\\n\
  float a = 1.0f / (1.0f + stretched[2]); \\\n\
  float r = native_sqrt(RXF); \\\n\
  float phi = (RYF < a) ? RYF / a * PI : PI + (RYF - a) / (1.0f - a) * PI; \\\n\
  float p1 = r * native_cos(phi); \\\n\
  float p2 = r * native_sin(phi) * ((RYF < a) ? 1.0f : stretched[2]); \\\n\
  N[0] = ROUGHNESS[0] * \\\n\
         (p1 * t1[0] + p2 * t2[0] + native_sqrt(1.0f - p1 * p1 - p2 * p2) * stretched[0]); \\\n\
  N[1] = ROUGHNESS[1] * \\\n\
         (p1 * t1[1] + p2 * t2[1] + native_sqrt(1.0f - p1 * p1 - p2 * p2) * stretched[1]); \\\n\
  N[2] = (p1 * t1[2] + p2 * t2[2] + native_sqrt(1.0f - p1 * p1 - p2 * p2) * stretched[2]); \\\n\
  NORMALIZE(N); \\\n\
}\n\
///\n\
#define TAN_THETA(V, T) \\\n\
{ \\\n\
  float temp = 1.0f - V[2] * V[2]; \\\n\
  if (temp <= 0.0f) { \\\n\
    T = 0.0f; \\\n\
  } else { \\\n\
    T = native_sqrt(temp) / V[2]; \\\n\
  } \\\n\
}\n\
///\n\
#define SIN_THETA_2(V, T) \\\n\
{ \\\n\
  T = 1.0f - V[2] * V[2]; \\\n\
}\n\
///\n\
#define PROJECT_ROUGHNESS(V, ROUGHNESS, R) \\\n\
{ \\\n\
  float sin_theta_2; \\\n\
  SIN_THETA_2(V, sin_theta_2); \\\n\
  float inv_sin_theta_2 = 1.0f / sin_theta_2; \\\n\
  if ((ROUGHNESS[0] == ROUGHNESS[1]) || (inv_sin_theta_2 <= 0.0f)) { \\\n\
    R = ROUGHNESS[0]; \\\n\
  } else { \\\n\
    float cos_phi_2 = V[0] * V[0] * inv_sin_theta_2; \\\n\
    float sin_phi_2 = V[1] * V[1] * inv_sin_theta_2; \\\n\
    R = native_sqrt(cos_phi_2 * ROUGHNESS[0] * ROUGHNESS[0] + \\\n\
             sin_phi_2 * ROUGHNESS[1] * ROUGHNESS[1]); \\\n\
  } \\\n\
}\n\
///\n\
#define HYPOT_2(A, B, R) \\\n\
{ \\\n\
  if (fabs(A) > fabs(B)) { \\\n\
    R = B / A; \\\n\
    R = fabs(A) * native_sqrt(1.0f + R * R); \\\n\
  } else if (B != 0.0f) { \\\n\
    R = A / B; \\\n\
    R = fabs(B) * native_sqrt(1.0f + R * R); \\\n\
  } else {\\\n\
    R = 0.0f; \\\n\
  } \\\n\
}\n\
///\n\
#define SMITH_G1(V, N, ROUGHNESS, G1) \\\n\
{ \\\n\
  if ((DOT(N, V) * V[2]) <= 0.0f) { \\\n\
    G1 = 0.0f; \\\n\
  } else { \\\n\
    float tan_theta; \\\n\
    TAN_THETA(V, tan_theta); \\\n\
    tan_theta = fabs(tan_theta); \\\n\
    if (tan_theta == 0.0f) { \\\n\
      G1 = 1.0f; \\\n\
    } else { \\\n\
      float alpha; \\\n\
      PROJECT_ROUGHNESS(V, ROUGHNESS, alpha); \\\n\
      float root = alpha * tan_theta; \\\n\
      float hypot_2; \\\n\
      HYPOT_2(1.0f, root, hypot_2); \\\n\
      G1 = 2.0f / (1.0f + hypot_2); \\\n\
    } \\\n\
  } \\\n\
}\n\
///\n\
#define GGX_EVAL(N, ROUGHNESS, R) \\\n\
{ \\\n\
  if (N[2] <= 0.0f) { \\\n\
    R = 0.0f; \\\n\
  } else { \\\n\
    float cos_theta_2 = N[2] * N[2]; \\\n\
    float exponent = ((N[0] * N[0]) / (ROUGHNESS[0] * ROUGHNESS[0]) + \\\n\
                      (N[1] * N[1]) / (ROUGHNESS[1] * ROUGHNESS[1])) / cos_theta_2; \\\n\
    float root = (1.0f + exponent) * cos_theta_2; \\\n\
    R = 1.0f / (PI * ROUGHNESS[0] * ROUGHNESS[1] * root * root); \\\n\
  } \\\n\
}\n\
///\n\
#define AVG_TONEMAP(RGB) \\\n\
{ \\\n\
  RGB[0] *= inv_sample_count; \\\n\
  RGB[1] *= inv_sample_count; \\\n\
  RGB[2] *= inv_sample_count; \\\n\
  RGB[0] *= image_properties.exposure + 2.0f; \\\n\
  RGB[1] *= image_properties.exposure + 2.0f; \\\n\
  RGB[2] *= image_properties.exposure + 2.0f; \\\n\
  if (image_properties.tonemap) { \\\n\
    RGB[0] = ((RGB[0] * (0.15f * RGB[0] + 0.10f * 0.5f) + 0.20f * 0.02f) / \\\n\
              (RGB[0] * (0.15f * RGB[0] + 0.5f) + 0.2f * 0.3f)) - (0.02f / 0.3f); \\\n\
    RGB[1] = ((RGB[1] * (0.15f * RGB[1] + 0.10f * 0.5f) + 0.20f * 0.02f) / \\\n\
              (RGB[1] * (0.15f * RGB[1] + 0.5f) + 0.2f * 0.3f)) - (0.02f / 0.3f); \\\n\
    RGB[2] = ((RGB[2] * (0.15f * RGB[2] + 0.10f * 0.5f) + 0.20f * 0.02f) / \\\n\
              (RGB[2] * (0.15f * RGB[2] + 0.5f) + 0.2f * 0.3f)) - (0.02f / 0.3f); \\\n\
    float whitescale = 11.2f; \\\n\
    whitescale = ((whitescale * (0.15f * whitescale + 0.10f * 0.5f) + \\\n\
                  0.20f * 0.02f) / (whitescale * (0.15f * whitescale + 0.5f) + \\\n\
                  0.2f * 0.3f)) - (0.02f / 0.3f); \\\n\
    whitescale = 1.0f / whitescale; \\\n\
    RGB[0] *= whitescale; \\\n\
    RGB[1] *= whitescale; \\\n\
    RGB[2] *= whitescale; \\\n\
  } \\\n\
  RGB[0] = pow(RGB[0], 1.0f / image_properties.gamma); \\\n\
  RGB[1] = pow(RGB[1], 1.0f / image_properties.gamma); \\\n\
  RGB[2] = pow(RGB[2], 1.0f / image_properties.gamma); \\\n\
}\n\
///\n\
#define MIS(A, B) ((A * A) / (A * A + B * B))\n\
///\n\
#define CONCENTRIC_HEMI(R0, R1, D) \\\n\
{ \\\n\
  R0 = 2.0f * R0 - 1.0f; \\\n\
  R1 = 2.0f * R1 - 1.0f; \\\n\
  float phi, r; \\\n\
  if (R0 == 0.0f && R1 == 0.0f) { \\\n\
    r = phi = 0.0f; \\\n\
  } else if ((R0 * R0) > (R1 * R1)) { \\\n\
    r = R0; \\\n\
    phi = (PI * 0.25f) * (R1 / (R0)); \\\n\
  } else { \\\n\
    r = R1; \\\n\
    phi = (PI * 0.5f) - (R0 / (R1)) * (PI * 0.25f); \\\n\
  } \\\n\
  float cos_phi = native_cos(phi); \\\n\
  float sin_phi = native_sin(phi); \\\n\
  D[0] = r * cos_phi; \\\n\
  D[1] = r * sin_phi; \\\n\
  D[2] = native_sqrt(fmax(0.0f, 1.0f - D[0] * D[0] - D[1] * D[1])); \\\n\
  NORMALIZE(D); \\\n\
}\n\
///\n\
#define GET_BASIS(N, T, B) \\\n\
{\\\n\
  float z = 1.0f + N[2]; \\\n\
  float a = 1.0f / NOT_ZERO(z); \\\n\
  float b = -N[0] * N[1] * a; \\\n\
  T[0] = 1.0f - N[0] * N[0] * a; \\\n\
  T[1] = b; \\\n\
  T[2] = -N[0]; \\\n\
  B[0] = b; \\\n\
  B[1] = 1.0f - N[1] * N[1] * a; \\\n\
  B[2] = -N[1]; \\\n\
}\n\
///\n\
#define GET_VT_BASIS(N, T, B) \\\n\
{\\\n\
  float flip_binormal; \\\n\
  if (use_triangles_image) { \\\n\
    T[0] = w * read_imagef(triangles_image, TRI_VT_0).x + \\\n\
           u * read_imagef(triangles_image, TRI_VT_1).x + \\\n\
           v * read_imagef(triangles_image, TRI_VT_2).x; \\\n\
    T[1] = w * read_imagef(triangles_image, TRI_VT_0 + 1).x + \\\n\
           u * read_imagef(triangles_image, TRI_VT_1 + 1).x + \\\n\
           v * read_imagef(triangles_image, TRI_VT_2 + 1).x; \\\n\
    T[2] = w * read_imagef(triangles_image, TRI_VT_0 + 2).x + \\\n\
           u * read_imagef(triangles_image, TRI_VT_1 + 2).x + \\\n\
           v * read_imagef(triangles_image, TRI_VT_2 + 2).x; \\\n\
    flip_binormal = w * read_imagef(triangles_image, TRI_VT_0 + 3).x + \\\n\
                    u * read_imagef(triangles_image, TRI_VT_1 + 3).x + \\\n\
                    v * read_imagef(triangles_image, TRI_VT_2 + 3).x; \\\n\
  } else { \\\n\
    T[0] = w * triangles[tri_index].vertex_tangents[0][0] + \\\n\
           u * triangles[tri_index].vertex_tangents[1][0] + \\\n\
           v * triangles[tri_index].vertex_tangents[2][0]; \\\n\
    T[1] = w * triangles[tri_index].vertex_tangents[0][1] + \\\n\
           u * triangles[tri_index].vertex_tangents[1][1] + \\\n\
           v * triangles[tri_index].vertex_tangents[2][1]; \\\n\
    T[2] = w * triangles[tri_index].vertex_tangents[0][2] + \\\n\
           u * triangles[tri_index].vertex_tangents[1][2] + \\\n\
           v * triangles[tri_index].vertex_tangents[2][2]; \\\n\
    flip_binormal = w * triangles[tri_index].vertex_tangents[0][3] + \\\n\
                    u * triangles[tri_index].vertex_tangents[1][3] + \\\n\
                    v * triangles[tri_index].vertex_tangents[2][3]; \\\n\
  } \\\n\
  CROSS(N, T, B); \\\n\
  if (flip_binormal < 0.0f) { \\\n\
    B[0] = -B[0]; \\\n\
    B[1] = -B[1]; \\\n\
    B[2] = -B[2]; \\\n\
  } \\\n\
}\n\
///\n\
#define RELATIVE_TO_BASIS(V, N, T, B, D) \\\n\
{\\\n\
  D[0] = DOT(V, T); \\\n\
  D[1] = DOT(V, B); \\\n\
  D[2] = DOT(V, N); \\\n\
}\n\
///\n\
#define ROTATE_TO_BASIS(V, N, T, B, D) \\\n\
{\\\n\
  D[0] = V[0] * T[0] + V[1] * B[0] + V[2] * N[0]; \\\n\
  D[1] = V[0] * T[1] + V[1] * B[1] + V[2] * N[1]; \\\n\
  D[2] = V[0] * T[2] + V[1] * B[2] + V[2] * N[2]; \\\n\
}\n\
///\n\
#define CONDUCTOR_FRESNEL(I, IOR, K, F) \\\n\
{ \\\n\
  float cos_theta_2 = I * I; \\\n\
  float temp = (IOR * IOR + K * K) * cos_theta_2; \\\n\
  float rp2 = (temp - (IOR * (2.0f * I)) + 1.0f) / \\\n\
              (temp + (IOR * (2.0f * I)) + 1.0f); \\\n\
  float tempf = IOR * IOR + K * K; \\\n\
  float rs2 = (tempf - (IOR * (2.0f * I)) + cos_theta_2) / \\\n\
              (tempf + (IOR * (2.0f * I)) + cos_theta_2); \\\n\
  F = 0.5f * (rp2 + rs2); \\\n\
}\n\
///\n\
#define DIELECTRIC_FRESNEL(I, IOR, COS_THETA_T, F) \\\n\
{ \\\n\
  float scale = (I > 0.0f) ? 1.0f / IOR : IOR; \\\n\
  float cos_theta_t2 = 1.0f - (1.0f - I * I) * (scale * scale); \\\n\
  if (cos_theta_t2 < 0.0f) { \\\n\
    COS_THETA_T = 0.0f; \\\n\
    F = 1.0f; \\\n\
  } else { \\\n\
    float cos_theta_i = fabs(I); \\\n\
    float _cos_theta_t = native_sqrt(cos_theta_t2); \\\n\
    float rs = (cos_theta_i - IOR * _cos_theta_t) / \\\n\
               (cos_theta_i + IOR * _cos_theta_t); \\\n\
    float rp = (IOR * cos_theta_i - _cos_theta_t) / \\\n\
               (IOR * cos_theta_i + _cos_theta_t); \\\n\
    COS_THETA_T = (I > 0.0f) ? -_cos_theta_t : _cos_theta_t; \\\n\
    F = 0.5f * (rs * rs + rp * rp); \\\n\
  } \\\n\
}\n\
///\n\
#define TRACE(USE_NODES_IMAGE, USE_TRIANGLES_IMAGE, PRIMARY_RAY) \\\n\
{ \\\n\
  float id[3], ood[3]; \\\n\
  id[0] = 1.0f / NOT_ZERO(d[0]); \\\n\
  id[1] = 1.0f / NOT_ZERO(d[1]); \\\n\
  id[2] = 1.0f / NOT_ZERO(d[2]); \\\n\
  ood[0] = o[0] * id[0]; \\\n\
  ood[1] = o[1] * id[1]; \\\n\
  ood[2] = o[2] * id[2]; \\\n\
  int h_tri_index = NO_TRI_HIT; \\\n\
  int h_material_type; \\\n\
  float h_u, h_v; \\\n\
  float h_t = FLT_MAX; \\\n\
  uint top_tree_height; \\\n\
  if (USE_NODES_IMAGE) { \\\n\
    int node_pixel = (top_tree_offset << 4); \\\n\
    top_tree_height = read_imageui(nodes_image, NODE_EXTRA_0).x; \\\n\
  } else { \\\n\
    top_tree_height = nodes[top_tree_offset].extra[0]; \\\n\
  } \\\n\
  uint tree_height = top_tree_height; \\\n\
  uint tree_offset = top_tree_offset; \\\n\
  uint node_offset = 0; \\\n\
  uint trail = 0x0; \\\n\
  uint key = 0x0; \\\n\
  uint level_bit = 0x1 << (top_tree_height - 1); \\\n\
  uint hit_leaf = TREE_NO_LEAF_HIT; \\\n\
  uint mesh_triangle_offset = 0; \\\n\
  uint hit_mask = HM_TEST_NODES; \\\n\
  while (true) { \\\n\
    while (hit_leaf >= TREE_NO_LEAF_HIT) { \\\n\
      uint node_index = node_offset + tree_offset; \\\n\
      int node_pixel = (node_index << 4); \\\n\
      if (level_bit == LB_NEW_SUB_TREE) { \\\n\
        if (USE_NODES_IMAGE) { \\\n\
          tree_height = read_imageui(nodes_image, NODE_EXTRA_0).x; \\\n\
          mesh_triangle_offset = read_imageui(nodes_image, NODE_EXTRA_1).x; \\\n\
        } else { \\\n\
          tree_height = nodes[node_index].extra[0]; \\\n\
          mesh_triangle_offset = nodes[node_index].extra[1]; \\\n\
        } \\\n\
        trail <<= tree_height; \\\n\
        key <<= tree_height; \\\n\
        level_bit = 0x1 << (tree_height - 1); \\\n\
      } \\\n\
      if (hit_mask == HM_TEST_NODES) { \\\n\
        float l_bounds[6], r_bounds[6]; \\\n\
        if (USE_NODES_IMAGE) { \\\n\
          l_bounds[0] = read_imagef(nodes_image, NODE_L_BOUNDS).x; \\\n\
          l_bounds[1] = read_imagef(nodes_image, NODE_L_BOUNDS + 1).x; \\\n\
          l_bounds[2] = read_imagef(nodes_image, NODE_L_BOUNDS + 2).x; \\\n\
          l_bounds[3] = read_imagef(nodes_image, NODE_L_BOUNDS + 3).x; \\\n\
          l_bounds[4] = read_imagef(nodes_image, NODE_L_BOUNDS + 4).x; \\\n\
          l_bounds[5] = read_imagef(nodes_image, NODE_L_BOUNDS + 5).x; \\\n\
          r_bounds[0] = read_imagef(nodes_image, NODE_R_BOUNDS).x; \\\n\
          r_bounds[1] = read_imagef(nodes_image, NODE_R_BOUNDS + 1).x; \\\n\
          r_bounds[2] = read_imagef(nodes_image, NODE_R_BOUNDS + 2).x; \\\n\
          r_bounds[3] = read_imagef(nodes_image, NODE_R_BOUNDS + 3).x; \\\n\
          r_bounds[4] = read_imagef(nodes_image, NODE_R_BOUNDS + 4).x; \\\n\
          r_bounds[5] = read_imagef(nodes_image, NODE_R_BOUNDS + 5).x; \\\n\
        } else { \\\n\
          l_bounds[0] = nodes[node_index].l_bounds[0]; \\\n\
          l_bounds[1] = nodes[node_index].l_bounds[1]; \\\n\
          l_bounds[2] = nodes[node_index].l_bounds[2]; \\\n\
          l_bounds[3] = nodes[node_index].l_bounds[3]; \\\n\
          l_bounds[4] = nodes[node_index].l_bounds[4]; \\\n\
          l_bounds[5] = nodes[node_index].l_bounds[5]; \\\n\
          r_bounds[0] = nodes[node_index].r_bounds[0]; \\\n\
          r_bounds[1] = nodes[node_index].r_bounds[1]; \\\n\
          r_bounds[2] = nodes[node_index].r_bounds[2]; \\\n\
          r_bounds[3] = nodes[node_index].r_bounds[3]; \\\n\
          r_bounds[4] = nodes[node_index].r_bounds[4]; \\\n\
          r_bounds[5] = nodes[node_index].r_bounds[5]; \\\n\
        } \\\n\
        float l_lo[3], l_hi[3]; \\\n\
        l_lo[0] = l_bounds[0] * id[0] - ood[0]; \\\n\
        l_lo[1] = l_bounds[1] * id[1] - ood[1]; \\\n\
        l_lo[2] = l_bounds[2] * id[2] - ood[2]; \\\n\
        l_hi[0] = l_bounds[3] * id[0] - ood[0]; \\\n\
        l_hi[1] = l_bounds[4] * id[1] - ood[1]; \\\n\
        l_hi[2] = l_bounds[5] * id[2] - ood[2]; \\\n\
        float r_lo[3], r_hi[3]; \\\n\
        r_lo[0] = r_bounds[0] * id[0] - ood[0]; \\\n\
        r_lo[1] = r_bounds[1] * id[1] - ood[1]; \\\n\
        r_lo[2] = r_bounds[2] * id[2] - ood[2]; \\\n\
        r_hi[0] = r_bounds[3] * id[0] - ood[0]; \\\n\
        r_hi[1] = r_bounds[4] * id[1] - ood[1]; \\\n\
        r_hi[2] = r_bounds[5] * id[2] - ood[2]; \\\n\
        float l_near = fmax(fmax(fmax(fmin(l_lo[0], l_hi[0]),\\\n\
                                      fmin(l_lo[1], l_hi[1])),\\\n\
                                      fmin(l_lo[2], l_hi[2])), 0.0f); \\\n\
        float l_far = fmin(fmin(fmax(l_lo[0], l_hi[0]),\\\n\
                                fmax(l_lo[1], l_hi[1])),\\\n\
                                fmax(l_lo[2], l_hi[2])); \\\n\
        float r_near = fmax(fmax(fmax(fmin(r_lo[0], r_hi[0]),\\\n\
                                      fmin(r_lo[1], r_hi[1])),\\\n\
                                      fmin(r_lo[2], r_hi[2])), 0.0f); \\\n\
        float r_far = fmin(fmin(fmax(r_lo[0], r_hi[0]),\\\n\
                                fmax(r_lo[1], r_hi[1])),\\\n\
                                fmax(r_lo[2], r_hi[2])); \\\n\
        hit_mask = (((l_far >= l_near) && (l_near < h_t)) << 1) | \\\n\
                    ((r_far >= r_near) && (r_near < h_t)); \\\n\
        if (hit_mask == HM_BOTH) { \\\n\
          hit_mask = (l_near < r_near) + 1; \\\n\
          trail |= level_bit; \\\n\
        } \\\n\
      } \\\n\
      if (hit_mask == HM_TEST_NODES) { \\\n\
        break; \\\n\
      } \\\n\
      if (hit_mask == HM_RIGHT) { \\\n\
        key |= level_bit; \\\n\
        node_offset += level_bit; \\\n\
      } else { \\\n\
        ++node_offset; \\\n\
      } \\\n\
      int child = (hit_mask & 0x1); \\\n\
      if (USE_NODES_IMAGE) { \\\n\
        child = read_imageui(nodes_image, NODE_L + child).x; \\\n\
      } else { \\\n\
        child = nodes[node_index].child[child]; \\\n\
      } \\\n\
      hit_mask = HM_TEST_NODES; \\\n\
      level_bit >>= 1; \\\n\
      if (child & TREE_LEAF) { \\\n\
        hit_leaf = (child & TREE_LEAF_MASK); \\\n\
        break; \\\n\
      } else if (child & TREE_SUB_TREE) { \\\n\
        tree_offset = (child & TREE_SUB_TREE_MASK); \\\n\
        level_bit = LB_NEW_SUB_TREE; \\\n\
        node_offset = 0; \\\n\
      } \\\n\
    } \\\n\
    if (hit_leaf != TREE_NO_LEAF_HIT) { \\\n\
      uint tri_count = (hit_leaf >> (31 - TREE_LEAF_TRIANGLE_FACTOR)); \\\n\
      uint tri_offset = mesh_triangle_offset + \\\n\
                        (hit_leaf & \\\n\
                        (TREE_LEAF_MASK >> TREE_LEAF_TRIANGLE_FACTOR)); \\\n\
      for (uint ti = tri_offset; ti <= tri_offset + tri_count; ++ti) { \\\n\
        int tri_pixel = ti * 42; \\\n\
        float vertex[3]; \\\n\
        float edges[2][3]; \\\n\
        float surface_normal[3]; \\\n\
        if (USE_TRIANGLES_IMAGE) { \\\n\
          vertex[0] = read_imagef(triangles_image, TRI_VERTEX).x; \\\n\
          vertex[1] = read_imagef(triangles_image, TRI_VERTEX + 1).x; \\\n\
          vertex[2] = read_imagef(triangles_image, TRI_VERTEX + 2).x; \\\n\
          edges[0][0] = read_imagef(triangles_image, TRI_EDGES_0).x; \\\n\
          edges[0][1] = read_imagef(triangles_image, TRI_EDGES_0 + 1).x; \\\n\
          edges[0][2] = read_imagef(triangles_image, TRI_EDGES_0 + 2).x; \\\n\
          edges[1][0] = read_imagef(triangles_image, TRI_EDGES_1).x; \\\n\
          edges[1][1] = read_imagef(triangles_image, TRI_EDGES_1 + 1).x; \\\n\
          edges[1][2] = read_imagef(triangles_image, TRI_EDGES_1 + 2).x; \\\n\
          surface_normal[0] = read_imagef(triangles_image, TRI_SURF_N).x; \\\n\
          surface_normal[1] = read_imagef(triangles_image, TRI_SURF_N + 1).x; \\\n\
          surface_normal[2] = read_imagef(triangles_image, TRI_SURF_N + 2).x; \\\n\
        } else { \\\n\
          vertex[0] = triangles[ti].vertex[0]; \\\n\
          vertex[1] = triangles[ti].vertex[1]; \\\n\
          vertex[2] = triangles[ti].vertex[2]; \\\n\
          edges[0][0] = triangles[ti].edges[0][0]; \\\n\
          edges[0][1] = triangles[ti].edges[0][1]; \\\n\
          edges[0][2] = triangles[ti].edges[0][2]; \\\n\
          edges[1][0] = triangles[ti].edges[1][0]; \\\n\
          edges[1][1] = triangles[ti].edges[1][1]; \\\n\
          edges[1][2] = triangles[ti].edges[1][2]; \\\n\
          surface_normal[0] = triangles[ti].surface_normal[0]; \\\n\
          surface_normal[1] = triangles[ti].surface_normal[1]; \\\n\
          surface_normal[2] = triangles[ti].surface_normal[2]; \\\n\
        } \\\n\
        float c[3], r[3]; \\\n\
        c[0] = vertex[0] - o[0]; \\\n\
        c[1] = vertex[1] - o[1]; \\\n\
        c[2] = vertex[2] - o[2]; \\\n\
        r[0] = (d[1] * c[2]) - (d[2] * c[1]); \\\n\
        r[1] = (d[2] * c[0]) - (d[0] * c[2]); \\\n\
        r[2] = (d[0] * c[1]) - (d[1] * c[0]); \\\n\
        float u, v; \\\n\
        u = r[0] * edges[1][0] + r[1] * edges[1][1] + \\\n\
            r[2] * edges[1][2]; \\\n\
        v = r[0] * edges[0][0] + r[1] * edges[0][1] + \\\n\
            r[2] * edges[0][2]; \\\n\
        float dot = DOT(-d, surface_normal); \\\n\
        float t = c[0] * surface_normal[0] + \\\n\
                  c[1] * surface_normal[1] + \\\n\
                  c[2] * surface_normal[2]; \\\n\
        float abs_dot = fabs(dot); \\\n\
        float sign_dot = copysign(1.0f, -dot); \\\n\
        float rcp_dot = 1.0f / NOT_ZERO(abs_dot); \\\n\
        u *= sign_dot; \\\n\
        v *= sign_dot; \\\n\
        t *= sign_dot * rcp_dot; \\\n\
        if ((dot != 0.0f) && (u >= 0.0f) && (v >= 0.0f) && \\\n\
            ((u + v) < abs_dot) && (t > 0.0f) && (t < h_t)) { \\\n\
          uint material_id; \\\n\
          float vertex_uvs[3][2]; \\\n\
          if (USE_TRIANGLES_IMAGE) { \\\n\
            material_id = read_imageui(triangles_image, TRI_MAT_ID).x; \\\n\
            vertex_uvs[0][0] = read_imagef(triangles_image, TRI_UV_0).x; \\\n\
            vertex_uvs[0][1] = read_imagef(triangles_image, TRI_UV_0 + 1).x; \\\n\
            vertex_uvs[1][0] = read_imagef(triangles_image, TRI_UV_1).x; \\\n\
            vertex_uvs[1][1] = read_imagef(triangles_image, TRI_UV_1 + 1).x; \\\n\
            vertex_uvs[2][0] = read_imagef(triangles_image, TRI_UV_2).x; \\\n\
            vertex_uvs[2][1] = read_imagef(triangles_image, TRI_UV_2 + 1).x; \\\n\
          } else { \\\n\
            material_id = triangles[ti].material_id; \\\n\
            vertex_uvs[0][0] = triangles[ti].vertex_uvs[0][0]; \\\n\
            vertex_uvs[0][1] = triangles[ti].vertex_uvs[0][1]; \\\n\
            vertex_uvs[1][0] = triangles[ti].vertex_uvs[1][0]; \\\n\
            vertex_uvs[1][1] = triangles[ti].vertex_uvs[1][1]; \\\n\
            vertex_uvs[2][0] = triangles[ti].vertex_uvs[2][0]; \\\n\
            vertex_uvs[2][1] = triangles[ti].vertex_uvs[2][1]; \\\n\
          } \\\n\
          u *= rcp_dot; \\\n\
          v *= rcp_dot; \\\n\
          float w = 1.0f - u - v; \\\n\
          float uv[2]; \\\n\
          uv[0] = w * vertex_uvs[0][0] + \\\n\
                  u * vertex_uvs[1][0] + \\\n\
                  v * vertex_uvs[2][0]; \\\n\
          uv[1] = w * vertex_uvs[0][1] + \\\n\
                  u * vertex_uvs[1][1] + \\\n\
                  v * vertex_uvs[2][1]; \\\n\
          if ((materials[material_id].glass > 0.0f) || \\\n\
              (materials[material_id].emission > 0.0f) || \\\n\
              (materials[material_id].double_sided) || (dot > 0.0f)) { \\\n\
            float alpha = 1.0f; \\\n\
            float alpha_sample = 0.0f; \\\n\
            float emission = materials[material_id].emission; \\\n\
            if (materials[material_id].map_flags & MAP_FLAG_EMISSION) { \\\n\
              float2 emission_uv; \\\n\
              emission_uv.x = uv[0]; \\\n\
              emission_uv.y = uv[1]; \\\n\
              emission_uv.x = \\\n\
                mix(materials[material_id].emission_bounds[0] + 0.5f, \\\n\
                     materials[material_id].emission_bounds[1] - 0.5f, \\\n\
                     emission_uv.x); \\\n\
              emission_uv.y = \\\n\
                mix(materials[material_id].emission_bounds[2] + 0.5f, \\\n\
                     materials[material_id].emission_bounds[3] - 0.5f, \\\n\
                     emission_uv.y); \\\n\
              const sampler_t sampler = CLK_NORMALIZED_COORDS_TRUE | \\\n\
                                        CLK_ADDRESS_REPEAT | CLK_FILTER_LINEAR; \\\n\
              float4 emission_map = read_imagef(emission_atlas, sampler, emission_uv); \\\n\
              emission *= fmax(fmax(emission_map.x, emission_map.y), \\\n\
                               emission_map.z); \\\n\
            } \\\n\
            if (emission == 0.0f) { \\\n\
              alpha = materials[material_id].albedo[3]; \\\n\
              PRNG(global_id, b_seeds.w, alpha_sample); \\\n\
              if (materials[material_id].map_flags & MAP_FLAG_ALBEDO) { \\\n\
                float2 albedo_uv; \\\n\
                albedo_uv.x = uv[0]; \\\n\
                albedo_uv.y = uv[1]; \\\n\
                albedo_uv.x = \\\n\
                  mix(materials[material_id].albedo_bounds[0] + 0.5f, \\\n\
                       materials[material_id].albedo_bounds[1] - 0.5f, \\\n\
                       albedo_uv.x); \\\n\
                albedo_uv.y = \\\n\
                  mix(materials[material_id].albedo_bounds[2] + 0.5f, \\\n\
                       materials[material_id].albedo_bounds[3] - 0.5f, \\\n\
                       albedo_uv.y); \\\n\
                const sampler_t sampler = CLK_NORMALIZED_COORDS_TRUE | \\\n\
                                          CLK_ADDRESS_REPEAT | CLK_FILTER_LINEAR; \\\n\
                float4 albedo_map = read_imagef(albedo_atlas, sampler, albedo_uv); \\\n\
                alpha *= albedo_map.w; \\\n\
              } \\\n\
            } \\\n\
            if (alpha_sample < alpha) { \\\n\
              h_t = t; \\\n\
              h_u = u; \\\n\
              h_v = v; \\\n\
              if (emission > 0.0f) { \\\n\
                h_material_type = STATE_EMISSIVE; \\\n\
              } else { \\\n\
                h_material_type = STATE_MATERIAL; \\\n\
              } \\\n\
              h_tri_index = ti; \\\n\
            } \\\n\
          } \\\n\
        } \\\n\
      } \\\n\
      hit_leaf = TREE_NO_LEAF_HIT; \\\n\
    } \\\n\
    if (trail == 0x0) { \\\n\
      break; \\\n\
    } \\\n\
    uint shift = 31 - clz(trail & -trail); \\\n\
    if (shift >= tree_height) { \\\n\
      shift -= tree_height; \\\n\
      trail >>= tree_height; \\\n\
      key >>= tree_height; \\\n\
      tree_height = top_tree_height; \\\n\
      tree_offset = top_tree_offset; \\\n\
      mesh_triangle_offset = 0; \\\n\
    } \\\n\
    level_bit = (0x1 << shift); \\\n\
    hit_mask = ((key & level_bit) > 0) + 1; \\\n\
    trail ^= level_bit; \\\n\
    key = (key & (0xFFFFFFFF << (shift + 1))); \\\n\
    uint local_key = key & (0xFFFFFFFF >> (32 - tree_height)); \\\n\
    node_offset = local_key + ((32 - popcount(local_key)) - \\\n\
                               (32 - tree_height) - (shift + 1)); \\\n\
  } \\\n\
  ray_state[RS_DIR_X] = d[0]; \\\n\
  ray_state[RS_DIR_Y] = d[1]; \\\n\
  ray_state[RS_DIR_Z] = d[2]; \\\n\
  if (h_tri_index != NO_TRI_HIT) {\\\n\
    ray_state[RS_STATE] = h_material_type; \\\n\
    o[0] += d[0] * h_t; \\\n\
    o[1] += d[1] * h_t; \\\n\
    o[2] += d[2] * h_t; \\\n\
    ray_state[RS_ORIGIN_X] = o[0]; \\\n\
    ray_state[RS_ORIGIN_Y] = o[1]; \\\n\
    ray_state[RS_ORIGIN_Z] = o[2]; \\\n\
    ray_state[RS_U] = h_u; \\\n\
    ray_state[RS_V] = h_v; \\\n\
    ray_state[RS_HIT_DISTANCE] = h_t; \\\n\
    ray_state[RS_TRI_INDEX] = h_tri_index; \\\n\
  } else {\\\n\
    ray_state[RS_STATE] = STATE_ENVIRONMENT; \\\n\
  }\\\n\
}\n\
///\n\
#define OCCLUSION(O, D, USE_NODES_IMAGE, USE_TRIANGLES_IMAGE) \\\n\
{ \\\n\
  float id[3], ood[3]; \\\n\
  id[0] = 1.0f / NOT_ZERO(D[0]); \\\n\
  id[1] = 1.0f / NOT_ZERO(D[1]); \\\n\
  id[2] = 1.0f / NOT_ZERO(D[2]); \\\n\
  ood[0] = O[0] * id[0]; \\\n\
  ood[1] = O[1] * id[1]; \\\n\
  ood[2] = O[2] * id[2]; \\\n\
  uint top_tree_height; \\\n\
  if (USE_NODES_IMAGE) { \\\n\
    int node_pixel = (top_tree_offset << 4); \\\n\
    top_tree_height = read_imageui(nodes_image, NODE_EXTRA_0).x; \\\n\
  } else { \\\n\
    top_tree_height = nodes[top_tree_offset].extra[0]; \\\n\
  } \\\n\
  uint tree_height = top_tree_height; \\\n\
  uint tree_offset = top_tree_offset; \\\n\
  uint node_offset = 0; \\\n\
  uint trail = 0x0; \\\n\
  uint key = 0x0; \\\n\
  uint level_bit = 0x1 << (top_tree_height - 1); \\\n\
  uint hit_leaf = TREE_NO_LEAF_HIT; \\\n\
  uint mesh_triangle_offset = 0; \\\n\
  uint hit_mask = HM_TEST_NODES; \\\n\
  while (true) { \\\n\
    while (hit_leaf >= TREE_NO_LEAF_HIT) { \\\n\
      uint node_index = node_offset + tree_offset; \\\n\
      int node_pixel = (node_index << 4); \\\n\
      if (level_bit == LB_NEW_SUB_TREE) { \\\n\
        if (USE_NODES_IMAGE) { \\\n\
          tree_height = read_imageui(nodes_image, NODE_EXTRA_0).x; \\\n\
          mesh_triangle_offset = read_imageui(nodes_image, NODE_EXTRA_1).x; \\\n\
        } else { \\\n\
          tree_height = nodes[node_index].extra[0]; \\\n\
          mesh_triangle_offset = nodes[node_index].extra[1]; \\\n\
        } \\\n\
        trail <<= tree_height; \\\n\
        key <<= tree_height; \\\n\
        level_bit = 0x1 << (tree_height - 1); \\\n\
      } \\\n\
      if (hit_mask == HM_TEST_NODES) { \\\n\
        float l_bounds[6], r_bounds[6]; \\\n\
        if (USE_NODES_IMAGE) { \\\n\
          l_bounds[0] = read_imagef(nodes_image, NODE_L_BOUNDS).x; \\\n\
          l_bounds[1] = read_imagef(nodes_image, NODE_L_BOUNDS + 1).x; \\\n\
          l_bounds[2] = read_imagef(nodes_image, NODE_L_BOUNDS + 2).x; \\\n\
          l_bounds[3] = read_imagef(nodes_image, NODE_L_BOUNDS + 3).x; \\\n\
          l_bounds[4] = read_imagef(nodes_image, NODE_L_BOUNDS + 4).x; \\\n\
          l_bounds[5] = read_imagef(nodes_image, NODE_L_BOUNDS + 5).x; \\\n\
          r_bounds[0] = read_imagef(nodes_image, NODE_R_BOUNDS).x; \\\n\
          r_bounds[1] = read_imagef(nodes_image, NODE_R_BOUNDS + 1).x; \\\n\
          r_bounds[2] = read_imagef(nodes_image, NODE_R_BOUNDS + 2).x; \\\n\
          r_bounds[3] = read_imagef(nodes_image, NODE_R_BOUNDS + 3).x; \\\n\
          r_bounds[4] = read_imagef(nodes_image, NODE_R_BOUNDS + 4).x; \\\n\
          r_bounds[5] = read_imagef(nodes_image, NODE_R_BOUNDS + 5).x; \\\n\
        } else { \\\n\
          l_bounds[0] = nodes[node_index].l_bounds[0]; \\\n\
          l_bounds[1] = nodes[node_index].l_bounds[1]; \\\n\
          l_bounds[2] = nodes[node_index].l_bounds[2]; \\\n\
          l_bounds[3] = nodes[node_index].l_bounds[3]; \\\n\
          l_bounds[4] = nodes[node_index].l_bounds[4]; \\\n\
          l_bounds[5] = nodes[node_index].l_bounds[5]; \\\n\
          r_bounds[0] = nodes[node_index].r_bounds[0]; \\\n\
          r_bounds[1] = nodes[node_index].r_bounds[1]; \\\n\
          r_bounds[2] = nodes[node_index].r_bounds[2]; \\\n\
          r_bounds[3] = nodes[node_index].r_bounds[3]; \\\n\
          r_bounds[4] = nodes[node_index].r_bounds[4]; \\\n\
          r_bounds[5] = nodes[node_index].r_bounds[5]; \\\n\
        } \\\n\
        float l_lo[3], l_hi[3]; \\\n\
        l_lo[0] = l_bounds[0] * id[0] - ood[0]; \\\n\
        l_lo[1] = l_bounds[1] * id[1] - ood[1]; \\\n\
        l_lo[2] = l_bounds[2] * id[2] - ood[2]; \\\n\
        l_hi[0] = l_bounds[3] * id[0] - ood[0]; \\\n\
        l_hi[1] = l_bounds[4] * id[1] - ood[1]; \\\n\
        l_hi[2] = l_bounds[5] * id[2] - ood[2]; \\\n\
        float r_lo[3], r_hi[3]; \\\n\
        r_lo[0] = r_bounds[0] * id[0] - ood[0]; \\\n\
        r_lo[1] = r_bounds[1] * id[1] - ood[1]; \\\n\
        r_lo[2] = r_bounds[2] * id[2] - ood[2]; \\\n\
        r_hi[0] = r_bounds[3] * id[0] - ood[0]; \\\n\
        r_hi[1] = r_bounds[4] * id[1] - ood[1]; \\\n\
        r_hi[2] = r_bounds[5] * id[2] - ood[2]; \\\n\
        float l_near = fmax(fmax(fmax(fmin(l_lo[0], l_hi[0]),\\\n\
                                      fmin(l_lo[1], l_hi[1])),\\\n\
                                      fmin(l_lo[2], l_hi[2])), 0.0f); \\\n\
        float l_far = fmin(fmin(fmax(l_lo[0], l_hi[0]),\\\n\
                                fmax(l_lo[1], l_hi[1])),\\\n\
                                fmax(l_lo[2], l_hi[2])); \\\n\
        float r_near = fmax(fmax(fmax(fmin(r_lo[0], r_hi[0]),\\\n\
                                      fmin(r_lo[1], r_hi[1])),\\\n\
                                      fmin(r_lo[2], r_hi[2])), 0.0f); \\\n\
        float r_far = fmin(fmin(fmax(r_lo[0], r_hi[0]),\\\n\
                                fmax(r_lo[1], r_hi[1])),\\\n\
                                fmax(r_lo[2], r_hi[2])); \\\n\
        hit_mask = (((l_far >= l_near) && (l_near < h_t)) << 1) | \\\n\
                    ((r_far >= r_near) && (r_near < h_t)); \\\n\
        if (hit_mask == HM_BOTH) { \\\n\
          hit_mask = (l_near < r_near) + 1; \\\n\
          trail |= level_bit; \\\n\
        } \\\n\
      } \\\n\
      if (hit_mask == HM_TEST_NODES) { \\\n\
        break; \\\n\
      } \\\n\
      if (hit_mask == HM_RIGHT) { \\\n\
        key |= level_bit; \\\n\
        node_offset += level_bit; \\\n\
      } else { \\\n\
        ++node_offset; \\\n\
      } \\\n\
      int child = (hit_mask & 0x1); \\\n\
      if (USE_NODES_IMAGE) { \\\n\
        child = read_imageui(nodes_image, NODE_L + child).x; \\\n\
      } else { \\\n\
        child = nodes[node_index].child[child]; \\\n\
      } \\\n\
      hit_mask = HM_TEST_NODES; \\\n\
      level_bit >>= 1; \\\n\
      if (child & TREE_LEAF) { \\\n\
        hit_leaf = (child & TREE_LEAF_MASK); \\\n\
        break; \\\n\
      } else if (child & TREE_SUB_TREE) { \\\n\
        tree_offset = (child & TREE_SUB_TREE_MASK); \\\n\
        level_bit = LB_NEW_SUB_TREE; \\\n\
        node_offset = 0; \\\n\
      } \\\n\
    } \\\n\
    if (hit_leaf != TREE_NO_LEAF_HIT) { \\\n\
      uint tri_count = (hit_leaf >> (31 - TREE_LEAF_TRIANGLE_FACTOR)); \\\n\
      uint tri_offset = mesh_triangle_offset + \\\n\
                        (hit_leaf & \\\n\
                        (TREE_LEAF_MASK >> TREE_LEAF_TRIANGLE_FACTOR)); \\\n\
      for (uint ti = tri_offset; ti <= tri_offset + tri_count; ++ti) { \\\n\
        int tri_pixel = ti * 42; \\\n\
        float vertex[3]; \\\n\
        float edges[2][3]; \\\n\
        float surface_normal[3]; \\\n\
        if (USE_TRIANGLES_IMAGE) { \\\n\
          vertex[0] = read_imagef(triangles_image, TRI_VERTEX).x; \\\n\
          vertex[1] = read_imagef(triangles_image, TRI_VERTEX + 1).x; \\\n\
          vertex[2] = read_imagef(triangles_image, TRI_VERTEX + 2).x; \\\n\
          edges[0][0] = read_imagef(triangles_image, TRI_EDGES_0).x; \\\n\
          edges[0][1] = read_imagef(triangles_image, TRI_EDGES_0 + 1).x; \\\n\
          edges[0][2] = read_imagef(triangles_image, TRI_EDGES_0 + 2).x; \\\n\
          edges[1][0] = read_imagef(triangles_image, TRI_EDGES_1).x; \\\n\
          edges[1][1] = read_imagef(triangles_image, TRI_EDGES_1 + 1).x; \\\n\
          edges[1][2] = read_imagef(triangles_image, TRI_EDGES_1 + 2).x; \\\n\
          surface_normal[0] = read_imagef(triangles_image, TRI_SURF_N).x; \\\n\
          surface_normal[1] = read_imagef(triangles_image, TRI_SURF_N + 1).x; \\\n\
          surface_normal[2] = read_imagef(triangles_image, TRI_SURF_N + 2).x; \\\n\
        } else { \\\n\
          vertex[0] = triangles[ti].vertex[0]; \\\n\
          vertex[1] = triangles[ti].vertex[1]; \\\n\
          vertex[2] = triangles[ti].vertex[2]; \\\n\
          edges[0][0] = triangles[ti].edges[0][0]; \\\n\
          edges[0][1] = triangles[ti].edges[0][1]; \\\n\
          edges[0][2] = triangles[ti].edges[0][2]; \\\n\
          edges[1][0] = triangles[ti].edges[1][0]; \\\n\
          edges[1][1] = triangles[ti].edges[1][1]; \\\n\
          edges[1][2] = triangles[ti].edges[1][2]; \\\n\
          surface_normal[0] = triangles[ti].surface_normal[0]; \\\n\
          surface_normal[1] = triangles[ti].surface_normal[1]; \\\n\
          surface_normal[2] = triangles[ti].surface_normal[2]; \\\n\
        } \\\n\
        float c[3], r[3]; \\\n\
        c[0] = vertex[0] - O[0]; \\\n\
        c[1] = vertex[1] - O[1]; \\\n\
        c[2] = vertex[2] - O[2]; \\\n\
        r[0] = (D[1] * c[2]) - (D[2] * c[1]); \\\n\
        r[1] = (D[2] * c[0]) - (D[0] * c[2]); \\\n\
        r[2] = (D[0] * c[1]) - (D[1] * c[0]); \\\n\
        float u, v; \\\n\
        u = r[0] * edges[1][0] + r[1] * edges[1][1] + \\\n\
            r[2] * edges[1][2]; \\\n\
        v = r[0] * edges[0][0] + r[1] * edges[0][1] + \\\n\
            r[2] * edges[0][2]; \\\n\
        float dot = DOT(-D, surface_normal); \\\n\
        float t = c[0] * surface_normal[0] + \\\n\
                  c[1] * surface_normal[1] + \\\n\
                  c[2] * surface_normal[2]; \\\n\
        float abs_dot = fabs(dot); \\\n\
        float sign_dot = copysign(1.0f, -dot); \\\n\
        float rcp_dot = 1.0f / NOT_ZERO(abs_dot); \\\n\
        u *= sign_dot; \\\n\
        v *= sign_dot; \\\n\
        t *= sign_dot * rcp_dot; \\\n\
        if ((dot != 0.0f) && (u >= 0.0f) && (v >= 0.0f) && \\\n\
            ((u + v) < abs_dot) && (t > 0.0f) && (t < h_t)) { \\\n\
          uint material_id; \\\n\
          float vertex_uvs[3][2]; \\\n\
          if (USE_TRIANGLES_IMAGE) { \\\n\
            material_id = read_imageui(triangles_image, TRI_MAT_ID).x; \\\n\
            vertex_uvs[0][0] = read_imagef(triangles_image, TRI_UV_0).x; \\\n\
            vertex_uvs[0][1] = read_imagef(triangles_image, TRI_UV_0 + 1).x; \\\n\
            vertex_uvs[1][0] = read_imagef(triangles_image, TRI_UV_1).x; \\\n\
            vertex_uvs[1][1] = read_imagef(triangles_image, TRI_UV_1 + 1).x; \\\n\
            vertex_uvs[2][0] = read_imagef(triangles_image, TRI_UV_2).x; \\\n\
            vertex_uvs[2][1] = read_imagef(triangles_image, TRI_UV_2 + 1).x; \\\n\
          } else { \\\n\
            material_id = triangles[ti].material_id; \\\n\
            vertex_uvs[0][0] = triangles[ti].vertex_uvs[0][0]; \\\n\
            vertex_uvs[0][1] = triangles[ti].vertex_uvs[0][1]; \\\n\
            vertex_uvs[1][0] = triangles[ti].vertex_uvs[1][0]; \\\n\
            vertex_uvs[1][1] = triangles[ti].vertex_uvs[1][1]; \\\n\
            vertex_uvs[2][0] = triangles[ti].vertex_uvs[2][0]; \\\n\
            vertex_uvs[2][1] = triangles[ti].vertex_uvs[2][1]; \\\n\
          } \\\n\
          u *= rcp_dot; \\\n\
          v *= rcp_dot; \\\n\
          float w = 1.0f - u - v; \\\n\
          float uv[2]; \\\n\
          uv[0] = w * vertex_uvs[0][0] + u * vertex_uvs[1][0] + \\\n\
                  v * vertex_uvs[2][0]; \\\n\
          uv[1] = w * vertex_uvs[0][1] + u * vertex_uvs[1][1] + \\\n\
                  v * vertex_uvs[2][1]; \\\n\
          if ((materials[material_id].glass > 0.0f) || \\\n\
              (materials[material_id].emission > 0.0f) || \\\n\
              (materials[material_id].double_sided) || (dot > 0.0f)) { \\\n\
            float alpha = 1.0f; \\\n\
            float alpha_sample = 0.0f; \\\n\
            float emission = materials[material_id].emission; \\\n\
            if (materials[material_id].map_flags & MAP_FLAG_EMISSION) { \\\n\
              float2 emission_uv; \\\n\
              emission_uv.x = uv[0]; \\\n\
              emission_uv.y = uv[1]; \\\n\
              emission_uv.x = \\\n\
                mix(materials[material_id].emission_bounds[0] + 0.5f, \\\n\
                    materials[material_id].emission_bounds[1] - 0.5f, \\\n\
                    emission_uv.x); \\\n\
              emission_uv.y = \\\n\
                mix(materials[material_id].emission_bounds[2] + 0.5f, \\\n\
                    materials[material_id].emission_bounds[3] - 0.5f, \\\n\
                    emission_uv.y); \\\n\
              const sampler_t sampler = CLK_NORMALIZED_COORDS_TRUE | \\\n\
                                        CLK_ADDRESS_REPEAT | CLK_FILTER_LINEAR; \\\n\
              float4 emission_map = read_imagef(emission_atlas, sampler, emission_uv); \\\n\
              emission *= fmax(fmax(emission_map.x, emission_map.y), \\\n\
                               emission_map.z); \\\n\
            } \\\n\
            if (emission == 0.0f) { \\\n\
              alpha = materials[material_id].albedo[3]; \\\n\
              PRNG(global_id, b_seeds.w, alpha_sample); \\\n\
              if (materials[material_id].map_flags & MAP_FLAG_ALBEDO) { \\\n\
                float2 albedo_uv; \\\n\
                albedo_uv.x = uv[0]; \\\n\
                albedo_uv.y = uv[1]; \\\n\
                albedo_uv.x = \\\n\
                  mix(materials[material_id].albedo_bounds[0] + 0.5f, \\\n\
                      materials[material_id].albedo_bounds[1] - 0.5f, \\\n\
                      albedo_uv.x); \\\n\
                albedo_uv.y = \\\n\
                  mix(materials[material_id].albedo_bounds[2] + 0.5f, \\\n\
                      materials[material_id].albedo_bounds[3] - 0.5f, \\\n\
                      albedo_uv.y); \\\n\
                const sampler_t sampler = CLK_NORMALIZED_COORDS_TRUE | \\\n\
                                          CLK_ADDRESS_REPEAT | CLK_FILTER_LINEAR; \\\n\
                float4 albedo_map = read_imagef(albedo_atlas, sampler, albedo_uv); \\\n\
                alpha *= albedo_map.w; \\\n\
              } \\\n\
            } \\\n\
            if (alpha_sample < alpha) { \\\n\
              h_t = t; \\\n\
              node_offset = TREE_TRAIL_END; \\\n\
              break; \\\n\
            } \\\n\
          } \\\n\
        } \\\n\
      } \\\n\
      if (node_offset == TREE_TRAIL_END) { \\\n\
        break; \\\n\
      } \\\n\
      hit_leaf = TREE_NO_LEAF_HIT; \\\n\
    } \\\n\
    if (node_offset == TREE_TRAIL_END) { \\\n\
      break; \\\n\
    } \\\n\
    if (trail == 0x0) { \\\n\
      break; \\\n\
    } \\\n\
    uint shift = 31 - clz(trail & -trail); \\\n\
    if (shift >= tree_height) { \\\n\
      shift -= tree_height; \\\n\
      trail >>= tree_height; \\\n\
      key >>= tree_height; \\\n\
      tree_height = top_tree_height; \\\n\
      tree_offset = top_tree_offset; \\\n\
      mesh_triangle_offset = 0; \\\n\
    } \\\n\
    level_bit = (0x1 << shift); \\\n\
    hit_mask = ((key & level_bit) > 0) + 1; \\\n\
    trail ^= level_bit; \\\n\
    key = (key & (0xFFFFFFFF << (shift + 1))); \\\n\
    uint local_key = key & (0xFFFFFFFF >> (32 - tree_height)); \\\n\
    node_offset = local_key + ((32 - popcount(local_key)) - \\\n\
                               (32 - tree_height) - (shift + 1)); \\\n\
  } \\\n\
}\n\
///\n\
kernel void Primary(global float* ray_state, \n\
                    global Material* materials,\n\
                    int use_nodes_image,\n\
                    read_only image1d_buffer_t nodes_image,\n\
                    global TwoNode* nodes,\n\
                    int top_tree_offset,\n\
                    int use_triangles_image,\n\
                    read_only image1d_buffer_t triangles_image,\n\
                    global TreeTriangle* triangles,\n\
                    read_only image2d_t albedo_atlas,\n\
                    read_only image2d_t emission_atlas,\n\
                    Camera camera,\n\
                    uint4  b_seeds,\n\
                    int width, \n\
                    int height, \n\
                    int invert_y,\n\
                    int sample)\n\
{\n\
  int global_id = get_global_id(1) * get_global_size(0) + get_global_id(0); \n\
  int ray_count = width * height;\n\
  if (global_id >= ray_count) {\n\
    return;\n\
  }\n\
  float o[3], d[3];\n\
  float pixel_sample[2];\n\
  PRNG(global_id, b_seeds.x, pixel_sample[0]);\n\
  PRNG(global_id, b_seeds.y, pixel_sample[1]);\n\
  int x_pixel = global_id % width;\n\
  int y_pixel = global_id / width;\n\
  if (invert_y) {\n\
    y_pixel = (height - 1) - y_pixel;\n\
  }\n\
  float p[3] = { camera.root_pixel[0] + ((x_pixel + pixel_sample[0]) * \n\
                                         camera.pixel_offset[0]),\n\
                 camera.root_pixel[1] + ((y_pixel + pixel_sample[1]) * \n\
                                         camera.pixel_offset[1]), \n\
                 1.0f };\n\
  p[0] *= camera.focal_depth;\n\
  p[1] *= camera.focal_depth;\n\
  p[2] *= camera.focal_depth;\n\
  float pt[3];\n\
  TRANSFORM_X(p, camera.transform, pt);\n\
  TRANSFORM_Y(p, camera.transform, pt);\n\
  TRANSFORM_Z(p, camera.transform, pt);\n\
  float aperture_sample[4];\n\
  PRNG(global_id, b_seeds.z, aperture_sample[0]);\n\
  PRNG(global_id, b_seeds.w, aperture_sample[1]);\n\
  float aperture_o[3];\n\
  CONCENTRIC_HEMI(aperture_sample[0], aperture_sample[1], aperture_o);\n\
  aperture_o[0] *= camera.aperture_size;\n\
  aperture_o[1] *= camera.aperture_size;\n\
  aperture_o[2] = 0.0f;\n\
  TRANSFORM_X(aperture_o, camera.transform, o);\n\
  TRANSFORM_Y(aperture_o, camera.transform, o);\n\
  TRANSFORM_Z(aperture_o, camera.transform, o);\n\
  d[0] = pt[0] - o[0];\n\
  d[1] = pt[1] - o[1];\n\
  d[2] = pt[2] - o[2];\n\
  NORMALIZE(d);\n\
  ray_state[RS_THROUGHPUT_R] = 1.0f;\n\
  ray_state[RS_THROUGHPUT_G] = 1.0f;\n\
  ray_state[RS_THROUGHPUT_B] = 1.0f;\n\
  if (sample == 0) {\n\
    ray_state[RS_COLOR_R] = 0.0f;\n\
    ray_state[RS_COLOR_G] = 0.0f;\n\
    ray_state[RS_COLOR_B] = 0.0f;\n\
  }\n\
	TRACE(use_nodes_image, use_triangles_image, true);\n\
	ray_state[RS_LAST_BRDF_PDF] = DIRAC_PDF;\n\
  return;\n\
}\n\
///\n\
kernel void Secondary(global float* ray_state, \n\
                      global Material* materials,\n\
                      int use_nodes_image,\n\
                      read_only image1d_buffer_t nodes_image,\n\
                      global TwoNode* nodes,\n\
                      int top_tree_offset,\n\
                      int use_triangles_image,\n\
                      read_only image1d_buffer_t triangles_image,\n\
                      global TreeTriangle* triangles,\n\
                      read_only image2d_t albedo_atlas,\n\
                      read_only image2d_t emission_atlas,\n\
                      uint4  b_seeds,\n\
                      int width, \n\
                      int height)\n\
{\n\
  int global_id = get_global_id(1) * get_global_size(0) + get_global_id(0); \n\
  int ray_count = width * height;\n\
  if (global_id >= ray_count) {\n\
    return;\n\
  }\n\
  int state = ray_state[RS_STATE];\n\
  if (state != STATE_SECONDARY) {\n\
    return;\n\
  }\n\
  float o[3], d[3];\n\
  d[0] = ray_state[RS_DIR_X];\n\
  d[1] = ray_state[RS_DIR_Y];\n\
  d[2] = ray_state[RS_DIR_Z];\n\
  o[0] = ray_state[RS_ORIGIN_X] + (d[0] * EPSILON);\n\
  o[1] = ray_state[RS_ORIGIN_Y] + (d[1] * EPSILON);\n\
  o[2] = ray_state[RS_ORIGIN_Z] + (d[2] * EPSILON);\n\
  TRACE(use_triangles_image, use_nodes_image, false);\n\
}\n\
\n\
///\n\
kernel void EnvSky(global float* ray_state,\n\
                   read_only image2d_t sun_table,\n\
                   Sun sun,\n\
                   int width,\n\
                   int height,\n\
                   int bounce)\n\
{\n\
  int global_id = get_global_id(0);\n\
  int ray_count = width * height;\n\
  if (global_id >= ray_count) {\n\
    return;\n\
  }\n\
  int state = ray_state[RS_STATE];\n\
  if (state != STATE_ENVIRONMENT) {\n\
    return;\n\
  }\n\
  float brdf_pdf = ray_state[RS_LAST_BRDF_PDF];\n\
  float d_i[3];\n\
  d_i[0] = ray_state[RS_DIR_X];\n\
  d_i[1] = ray_state[RS_DIR_Y];\n\
  d_i[2] = ray_state[RS_DIR_Z];\n\
  float s = -6360.5f * d_i[1];\n\
  float mu = -s * 0.00015722034f;\n\
  float nu = d_i[0] * sun.direction[0] + d_i[1] * sun.direction[1] + \n\
             d_i[2] * sun.direction[2];\n\
  float mus = (6360.5f * sun.direction[1]) * 0.00015722034f;\n\
  float rmu = 6360.5f * mu;\n\
  float delta = rmu * rmu - 6360.25f;\n\
  float4 cst;\n\
  if ((rmu < 0.0f) && (delta > 0.0f)) {\n\
    cst = (float4)(1.0f, 0.0f, 0.0f, 0.484375f);\n\
  } else {\n\
    cst = (float4)(-1.0f, 766800.000001f, 875.67117116f, 0.515625f);\n\
  }\n\
  float umu = cst.w + (rmu * cst.x + native_sqrt(delta + cst.y)) /\n\
              (79.7511755399f + cst.z) * 0.4921875f;\n\
  float umus = 0.015625f + \n\
               (atan(fmax(mus, -0.1975f) * 5.34962349919f) * 0.90909090f + \n\
               0.74f) * 0.484375f;\n\
  float lerp_value = (nu + 1.0f) * 3.5f;\n\
  float unu = floor(lerp_value);\n\
  lerp_value = lerp_value - unu;\n\
  float2 uv0 = (float2)((unu + umus) * 0.125f, umu);\n\
  float2 uv1 = (float2)((unu + umus + 1.0f) * 0.125f, umu);\n\
	const sampler_t sampler = CLK_NORMALIZED_COORDS_TRUE |\n\
                            CLK_ADDRESS_REPEAT | CLK_FILTER_LINEAR;\n\
  float4 raymie = fmax((read_imagef(sun_table, sampler, uv0) * (1.0f - lerp_value)) +\n\
                       (read_imagef(sun_table, sampler, uv1) * lerp_value), 0.0f);\n\
  float pr = 0.05968310365f * (1.0f + nu * nu);\n\
  float pm = 0.04297183463f * pow(1.64f - 1.6f * nu, -1.5f) * \n\
             (1.0f + nu * nu) * 0.378378378f;\n\
  float4 beta_r = (float4)(0.0058f, 0.0135f, 0.0331f, 1.0f);\n\
  float4 mie = raymie * raymie.w / (raymie.x) * (beta_r.x / beta_r) * \\\n\
                (1.0f + (d_i[1] * 3.0f));\n\
  float4 result = fmax(raymie * pr + mie * pm, (float4)(0.0f, 0.0f, 0.0f, 0.0f)) * \n\
                  30.0f * sun.sky_intensity;\n\
  float sun_delta = sun.direction[0] * d_i[0] + sun.direction[1] * d_i[1] + \n\
                    sun.direction[2] * d_i[2];\n\
  float color[3] = { result.x, result.y, result.z };\n\
  float mis = 1.0f;\n\
  if ((brdf_pdf != -1.0f) && (sun_delta >= SUN_COS_THETA)) {\n\
    float sun_pdf = 1.0f / (TAU * (1.0f - SUN_COS_THETA));\n\
    if (bounce > 0) {\n\
      if (brdf_pdf < DIRAC_PDF) {\n\
        if (sun.sun_intensity > 0.0f) {\n\
          mis = MIS(brdf_pdf, sun_pdf);\n\
        } else {\n\
          sun.sun_intensity *= 0.05f;\n\
        }\n\
      } else {\n\
        if (bounce > 5) {\n\
          ray_state[RS_STATE] = STATE_END;\n\
          return;\n\
        }\n\
        sun.sun_intensity *= 0.05f;\n\
      }\n\
    } else {\n\
      sun.sun_intensity *= 0.05f;\n\
    }\n\
    float horizon_falloff = fabs(d_i[1] - 0.075f) - (d_i[1] - 0.075f);\n\
    horizon_falloff = pow(horizon_falloff, 4.0f);\n\
    float hfo = horizon_falloff * 1000.0f;\n\
    sun.sun_intensity *= 1.0f + (sun.direction[1] * 4.0f);\n\
    float multiplier = clamp(1.0f / NOT_ZERO(hfo), 1.0f,\n\
                             fmax(5.0f, sun.sun_intensity * 100.0f));\n\
    color[0] *= multiplier;\n\
    color[1] *= multiplier;\n\
    color[2] *= multiplier;\n\
  } else if (bounce > 0) {\n\
    float horizon_bias = fmin(0.5f, \n\
                              (fmax(0.0f, sun.direction[1] - 0.1f) * 10.0f));\n\
    float max_component = fmax(color[0], fmax(color[1], color[2]));\n\
    color[0] = mix(color[0], max_component * 1.0f, horizon_bias);\n\
    color[1] = mix(color[1], max_component * 0.98f, horizon_bias); \n\
    color[2] = mix(color[2], max_component * 0.73f, horizon_bias); \n\
    color[0] *= sun.color[0];\n\
    color[1] *= sun.color[1];\n\
    color[2] *= sun.color[2];\n\
  }\n\
  ray_state[RS_COLOR_R] += ray_state[RS_THROUGHPUT_R] * color[0] * mis;\n\
  ray_state[RS_COLOR_G] += ray_state[RS_THROUGHPUT_G] * color[1] * mis;\n\
  ray_state[RS_COLOR_B] += ray_state[RS_THROUGHPUT_B] * color[2] * mis;\n\
  ray_state[RS_STATE] = STATE_END;\n\
  return;\n\
}\n\
///\n\
kernel void EnvMap(global float* ray_state,\n\
                   read_only image2d_t environment_map,\n\
                   EnvironmentProperties environment_properties,\n\
                   int width,\n\
                   int height,\n\
                   int bounce)\n\
{\n\
  int global_id = get_global_id(0); \n\
  int ray_count = width * height;\n\
  if (global_id >= ray_count) {\n\
    return;\n\
  }\n\
  int state = ray_state[RS_STATE];\n\
  if (state != STATE_ENVIRONMENT) {\n\
    return;\n\
  }\n\
  float d_i[3];\n\
  d_i[0] = ray_state[RS_DIR_X];\n\
  d_i[1] = ray_state[RS_DIR_Y];\n\
  d_i[2] = ray_state[RS_DIR_Z];\n\
  NORMALIZE(d_i);\n\
  float2 uv = (float2)(1.0f + atan2(d_i[0], -d_i[2]), acos(-d_i[1]) * INV_PI);\n\
  uv.x *= INV_TAU;\n\
  uv.x += environment_properties.rotation;\n\
  const sampler_t sampler = CLK_NORMALIZED_COORDS_TRUE |\n\
                            CLK_ADDRESS_REPEAT | CLK_FILTER_LINEAR;\n\
  float4 color = read_imagef(environment_map, sampler, uv);\n\
	color.x *= environment_properties.intensity;\n\
	color.y *= environment_properties.intensity;\n\
	color.z *= environment_properties.intensity;\n\
  ray_state[RS_COLOR_R] += ray_state[RS_THROUGHPUT_R] * color.x;\n\
  ray_state[RS_COLOR_G] += ray_state[RS_THROUGHPUT_G] * color.y;\n\
  ray_state[RS_COLOR_B] += ray_state[RS_THROUGHPUT_B] * color.z;\n\
  ray_state[RS_STATE] = STATE_END;\n\
  return;\n\
\n\
}\n\
///\n\
kernel void Emission(global float* ray_state,\n\
                     global Material* materials,\n\
                     int use_triangles_image,\n\
                     read_only image1d_buffer_t triangles_image,\n\
                     global TreeTriangle* triangles,\n\
                     read_only image2d_t emission_atlas,\n\
                     int emitter_count,\n\
                     int width,\n\
                     int height,\n\
                     int bounce)\n\
{\n\
  int global_id = get_global_id(0);\n\
  int ray_count = width * height;\n\
  if (global_id >= ray_count) {\n\
    return;\n\
  }\n\
  int state = ray_state[RS_STATE];\n\
  if (state != STATE_EMISSIVE) {\n\
    return;\n\
  }\n\
  float d[3];\n\
  d[0] = ray_state[RS_DIR_X];\n\
  d[1] = ray_state[RS_DIR_Y];\n\
  d[2] = ray_state[RS_DIR_Z];\n\
  float u = ray_state[RS_U];\n\
  float v = ray_state[RS_V];\n\
  float w = 1.0f - u - v;\n\
  int tri_index = ray_state[RS_TRI_INDEX];\n\
  int tri_pixel = tri_index * 42;\n\
  float vn[3];\n\
  if (use_triangles_image) {\n\
    vn[0] = w * read_imagef(triangles_image, TRI_VN_0).x + \n\
            u * read_imagef(triangles_image, TRI_VN_1).x + \n\
            v * read_imagef(triangles_image, TRI_VN_2).x;\n\
    vn[1] = w * read_imagef(triangles_image, TRI_VN_0 + 1).x + \n\
            u * read_imagef(triangles_image, TRI_VN_1 + 1).x + \n\
            v * read_imagef(triangles_image, TRI_VN_2 + 1).x;\n\
    vn[2] = w * read_imagef(triangles_image, TRI_VN_0 + 2).x + \n\
            u * read_imagef(triangles_image, TRI_VN_1 + 2).x + \n\
            v * read_imagef(triangles_image, TRI_VN_2 + 2).x;\n\
  } else {\n\
    vn[0] = w * triangles[tri_index].vertex_normals[0][0] +\n\
            u * triangles[tri_index].vertex_normals[1][0] + \n\
            v * triangles[tri_index].vertex_normals[2][0];\n\
    vn[1] = w * triangles[tri_index].vertex_normals[0][1] +\n\
            u * triangles[tri_index].vertex_normals[1][1] + \n\
            v * triangles[tri_index].vertex_normals[2][1];\n\
    vn[2] = w * triangles[tri_index].vertex_normals[0][2] +\n\
            u * triangles[tri_index].vertex_normals[1][2] + \n\
            v * triangles[tri_index].vertex_normals[2][2];\n\
  }\n\
  NORMALIZE(vn); \n\
  float dot_ni = DOT(-d, vn);\n\
  if (dot_ni <= 0.0f) {\n\
    ray_state[RS_STATE] = STATE_END;\n\
    return;\n\
  } else {\n\
    uint mat_id;\n\
    float uv[2];\n\
    float inverse_area;\n\
    if (use_triangles_image) {\n\
      mat_id = read_imageui(triangles_image, TRI_MAT_ID).x;\n\
      uv[0] = w * read_imagef(triangles_image, TRI_UV_0).x +\n\
              u * read_imagef(triangles_image, TRI_UV_1).x +\n\
              v * read_imagef(triangles_image, TRI_UV_2).x;\n\
      uv[1] = w * read_imagef(triangles_image, TRI_UV_0 + 1).x +\n\
              u * read_imagef(triangles_image, TRI_UV_1 + 1).x +\n\
              v * read_imagef(triangles_image, TRI_UV_2 + 1).x;\n\
      inverse_area = read_imagef(triangles_image, TRI_INV_AREA).x;\n\
    } else {\n\
      mat_id = triangles[tri_index].material_id;\n\
      uv[0] = w * triangles[tri_index].vertex_uvs[0][0] +\n\
              u * triangles[tri_index].vertex_uvs[1][0] +\n\
              v * triangles[tri_index].vertex_uvs[2][0];\n\
      uv[1] = w * triangles[tri_index].vertex_uvs[0][1] +\n\
              u * triangles[tri_index].vertex_uvs[1][1] +\n\
              v * triangles[tri_index].vertex_uvs[2][1];\n\
      inverse_area = triangles[tri_index].inverse_area; \n\
    }\n\
    float t = ray_state[RS_HIT_DISTANCE];\n\
    float t_2 = t * t;\n\
    float emitter_color[3];\n\
    float intensity = materials[mat_id].emission * 25.0f;\n\
    if (materials[mat_id].map_flags & MAP_FLAG_EMISSION) {\n\
      float2 emission_uv;\n\
      emission_uv.x = uv[0];\n\
      emission_uv.y = uv[1];\n\
      emission_uv.x = mix(materials[mat_id].emission_bounds[0] + 0.5f,\n\
                           materials[mat_id].emission_bounds[1] - 0.5f, \n\
                           emission_uv.x);\n\
      emission_uv.y = mix(materials[mat_id].emission_bounds[2] + 0.5f,\n\
                           materials[mat_id].emission_bounds[3] - 0.5f, \n\
                           emission_uv.y);\n\
      const sampler_t sampler = CLK_NORMALIZED_COORDS_TRUE |\n\
                                CLK_ADDRESS_REPEAT | CLK_FILTER_LINEAR;\n\
      float4 emission_map = read_imagef(emission_atlas, sampler, emission_uv);\n\
      emitter_color[0] = emission_map.x * intensity;\n\
      emitter_color[1] = emission_map.y * intensity;\n\
      emitter_color[2] = emission_map.z * intensity;\n\
    } else {\n\
      emitter_color[0] = materials[mat_id].albedo[0] * intensity;\n\
      emitter_color[1] = materials[mat_id].albedo[1] * intensity;\n\
      emitter_color[2] = materials[mat_id].albedo[2] * intensity;\n\
    }\n\
    float brdf_pdf = ray_state[RS_LAST_BRDF_PDF];\n\
    if (brdf_pdf == -1.0f) {\n\
      ray_state[RS_STATE] = STATE_END;\n\
      return;\n\
    }\n\
    float emitter_prob = 1.0f / (float)emitter_count; \n\
    float emitter_pdf = inverse_area *\n\
                        (t_2 / fabs(dot_ni)) * emitter_prob;\n\
    float mis = 1.0f;\n\
    if (bounce > 0) {\n\
      if (brdf_pdf < DIRAC_PDF) {\n\
        mis = MIS(brdf_pdf, emitter_pdf);\n\
      } else if (bounce > 5) {\n\
        ray_state[RS_STATE] = STATE_END;\n\
        return;\n\
      }\n\
    }\n\
    ray_state[RS_COLOR_R] += ray_state[RS_THROUGHPUT_R] * emitter_color[0] * mis;\n\
    ray_state[RS_COLOR_G] += ray_state[RS_THROUGHPUT_G] * emitter_color[1] * mis;\n\
    ray_state[RS_COLOR_B] += ray_state[RS_THROUGHPUT_B] * emitter_color[2] * mis;\n\
    ray_state[RS_STATE] = STATE_END;\n\
    return;\n\
  }\n\
}\n\
///\n\
kernel void Sample(global float* ray_state,\n\
                   global Material* materials,\n\
                   int use_triangles_image,\n\
                   read_only image1d_buffer_t triangles_image,\n\
                   global TreeTriangle* triangles,\n\
                   read_only image2d_t albedo_atlas,\n\
                   read_only image2d_t normal_atlas,\n\
                   read_only image2d_t metallic_atlas,\n\
                   read_only image2d_t emission_atlas,\n\
                   read_only image2d_t sun_table,\n\
                   Sun sun,\n\
                   read_only image2d_t environment_map,\n\
                   EnvironmentProperties environment_properties,\n\
                   global EmitterIndex* emitters,\n\
                   int emitter_count,\n\
                   global PortalIndex* portals,\n\
                   int portal_count,\n\
                   uint4 b_seeds_0,\n\
                   uint4 b_seeds_1,\n\
                   uint4 b_seeds_2,\n\
                   uint4 b_seeds_3,\n\
                   int width,\n\
                   int height,\n\
                   int sample_sun)\n\
{\n\
  int global_id = get_global_id(0); \n\
  int ray_count = width * height;\n\
  if (global_id >= ray_count) {\n\
    return;\n\
  }\n\
  int state = ray_state[RS_STATE];\n\
  if (state == STATE_END) {\n\
    return;\n\
  }\n\
  float last_brdf_pdf;\n\
  float new_brdf_pdf;\n\
  float sn[3];\n\
  float vn[3];\n\
  float n[3];\n\
  float n_tangent[3];\n\
  float n_binormal[3];\n\
  float ld_i[3];\n\
  float old_throughput[3];\n\
  float albedo[3];\n\
  float metallic;\n\
  float smoothness;\n\
  bool glass_material;\n\
  {\n\
    float u = ray_state[RS_U];\n\
    float v = ray_state[RS_V];\n\
    float w = 1.0f - u - v;\n\
    uint tri_index = ray_state[RS_TRI_INDEX];\n\
    int tri_pixel = tri_index * 42;\n\
    uint mat_id;\n\
    float uv[2];\n\
    if (use_triangles_image) {\n\
      mat_id = read_imageui(triangles_image, TRI_MAT_ID).x;\n\
      sn[0] = read_imagef(triangles_image, TRI_SURF_N).x;\n\
      sn[1] = read_imagef(triangles_image, TRI_SURF_N + 1).x;\n\
      sn[2] = read_imagef(triangles_image, TRI_SURF_N + 2).x;\n\
      uv[0] = w * read_imagef(triangles_image, TRI_UV_0).x +\n\
              u * read_imagef(triangles_image, TRI_UV_1).x +\n\
              v * read_imagef(triangles_image, TRI_UV_2).x;\n\
      uv[1] = w * read_imagef(triangles_image, TRI_UV_0 + 1).x +\n\
              u * read_imagef(triangles_image, TRI_UV_1 + 1).x +\n\
              v * read_imagef(triangles_image, TRI_UV_2 + 1).x;\n\
      vn[0] = w * read_imagef(triangles_image, TRI_VN_0).x + \n\
              u * read_imagef(triangles_image, TRI_VN_1).x + \n\
              v * read_imagef(triangles_image, TRI_VN_2).x;\n\
      vn[1] = w * read_imagef(triangles_image, TRI_VN_0 + 1).x + \n\
              u * read_imagef(triangles_image, TRI_VN_1 + 1).x + \n\
              v * read_imagef(triangles_image, TRI_VN_2 + 1).x;\n\
      vn[2] = w * read_imagef(triangles_image, TRI_VN_0 + 2).x + \n\
              u * read_imagef(triangles_image, TRI_VN_1 + 2).x + \n\
              v * read_imagef(triangles_image, TRI_VN_2 + 2).x;\n\
    } else {\n\
      mat_id = triangles[tri_index].material_id;\n\
      sn[0] = triangles[tri_index].surface_normal[0];\n\
      sn[1] = triangles[tri_index].surface_normal[1];\n\
      sn[2] = triangles[tri_index].surface_normal[2];\n\
      uv[0] = w * triangles[tri_index].vertex_uvs[0][0] +\n\
              u * triangles[tri_index].vertex_uvs[1][0] +\n\
              v * triangles[tri_index].vertex_uvs[2][0];\n\
      uv[1] = w * triangles[tri_index].vertex_uvs[0][1] +\n\
              u * triangles[tri_index].vertex_uvs[1][1] +\n\
              v * triangles[tri_index].vertex_uvs[2][1];\n\
      vn[0] = w * triangles[tri_index].vertex_normals[0][0] +\n\
              u * triangles[tri_index].vertex_normals[1][0] + \n\
              v * triangles[tri_index].vertex_normals[2][0];\n\
      vn[1] = w * triangles[tri_index].vertex_normals[0][1] +\n\
              u * triangles[tri_index].vertex_normals[1][1] + \n\
              v * triangles[tri_index].vertex_normals[2][1];\n\
      vn[2] = w * triangles[tri_index].vertex_normals[0][2] +\n\
              u * triangles[tri_index].vertex_normals[1][2] + \n\
              v * triangles[tri_index].vertex_normals[2][2];\n\
    }\n\
    glass_material = (materials[mat_id].glass > 0.0f);\n\
    float d_i[3];\n\
    d_i[0] = -ray_state[RS_DIR_X];\n\
    d_i[1] = -ray_state[RS_DIR_Y];\n\
    d_i[2] = -ray_state[RS_DIR_Z];\n\
    albedo[0] = materials[mat_id].albedo[0];\n\
    albedo[1] = materials[mat_id].albedo[1];\n\
    albedo[2] = materials[mat_id].albedo[2];\n\
    if (materials[mat_id].map_flags & MAP_FLAG_ALBEDO) {\n\
      float2 albedo_uv;\n\
      albedo_uv.x = uv[0];\n\
      albedo_uv.y = uv[1];\n\
      albedo_uv.x = mix(materials[mat_id].albedo_bounds[0] + 0.5f,\n\
                         materials[mat_id].albedo_bounds[1] - 0.5f, \n\
                         albedo_uv.x);\n\
      albedo_uv.y = mix(materials[mat_id].albedo_bounds[2] + 0.5f,\n\
                         materials[mat_id].albedo_bounds[3] - 0.5f, \n\
                         albedo_uv.y);\n\
      const sampler_t sampler = CLK_NORMALIZED_COORDS_TRUE |\n\
                                CLK_ADDRESS_REPEAT | CLK_FILTER_LINEAR;\n\
      float4 albedo_map = read_imagef(albedo_atlas, sampler, albedo_uv);\n\
      albedo[0] *= albedo_map.x;\n\
      albedo[1] *= albedo_map.y;\n\
      albedo[2] *= albedo_map.z;\n\
    }\n\
    if (materials[mat_id].map_flags & MAP_FLAG_NORMAL) {\n\
      float2 normal_uv;\n\
      normal_uv.x = uv[0];\n\
      normal_uv.y = uv[1];\n\
      normal_uv.x = mix(materials[mat_id].normal_bounds[0] + 0.5f,\n\
                         materials[mat_id].normal_bounds[1] - 0.5f, \n\
                         normal_uv.x);\n\
      normal_uv.y = mix(materials[mat_id].normal_bounds[2] + 0.5f,\n\
                         materials[mat_id].normal_bounds[3] - 0.5f, \n\
                         normal_uv.y);\n\
      const sampler_t sampler = CLK_NORMALIZED_COORDS_TRUE |\n\
                                CLK_ADDRESS_REPEAT | CLK_FILTER_LINEAR;\n\
      float4 normal_map = read_imagef(normal_atlas, sampler, normal_uv);\n\
      float vn_tangent[3], vn_binormal[3];\n\
      GET_VT_BASIS(vn, vn_tangent, vn_binormal);\n\
      float ln[3];\n\
      ln[0] = normal_map.w * 2.0f - 1.0f;\n\
      ln[1] = normal_map.y * 2.0f - 1.0f;\n\
      ln[2] = native_sqrt(1.0f - \n\
              clamp(ln[0] * ln[0] + ln[1] * ln[1], 0.0f, 1.0f));\n\
      ROTATE_TO_BASIS(ln, vn, vn_tangent, vn_binormal, n);\n\
    } else {\n\
      n[0] = vn[0];\n\
      n[1] = vn[1];\n\
      n[2] = vn[2];\n\
    }\n\
    NORMALIZE(n);\n\
    if (!glass_material && (DOT(d_i, sn) < 0.0f)) {\n\
      n[0] = -n[0];\n\
      n[1] = -n[1];\n\
      n[2] = -n[2];\n\
    }\n\
    metallic = materials[mat_id].metallic;\n\
    smoothness = materials[mat_id].smoothness;\n\
    if (materials[mat_id].map_flags & MAP_FLAG_METALLIC) {\n\
      float2 metallic_uv;\n\
      metallic_uv.x = uv[0];\n\
      metallic_uv.y = uv[1];\n\
      metallic_uv.x = mix(materials[mat_id].metallic_bounds[0] + 0.5f,\n\
                           materials[mat_id].metallic_bounds[1] - 0.5f, \n\
                           metallic_uv.x);\n\
      metallic_uv.y = mix(materials[mat_id].metallic_bounds[2] + 0.5f,\n\
                           materials[mat_id].metallic_bounds[3] - 0.5f, \n\
                           metallic_uv.y);\n\
      const sampler_t sampler = CLK_NORMALIZED_COORDS_TRUE |\n\
                                CLK_ADDRESS_REPEAT | CLK_FILTER_LINEAR;\n\
      float4 metallic_map = read_imagef(metallic_atlas, sampler, metallic_uv);\n\
      metallic *= metallic_map.x;\n\
      smoothness *= metallic_map.w;\n\
    }\n\
		float roughness[2] = { 1.0f - smoothness,\n\
                           1.0f - smoothness };\n\
    GET_BASIS(n, n_tangent, n_binormal);\n\
    RELATIVE_TO_BASIS(d_i, n, n_tangent, n_binormal, ld_i);\n\
    float ggx_pdf;\n\
		float ln_facet[3];\n\
		if (smoothness == 1.0f) {\n\
			ln_facet[0] = 0.0f;\n\
			ln_facet[1] = 0.0f;\n\
			ln_facet[2] = 1.0f;\n\
      ggx_pdf = DIRAC_PDF;\n\
    } else {\n\
			float ggx_samples[2];\n\
		  PRNG(global_id, b_seeds_0.x, ggx_samples[0]);\n\
		  PRNG(global_id, b_seeds_0.y, ggx_samples[1]);\n\
			GGX_SAMPLE(ld_i, roughness, ggx_samples[0], ggx_samples[1], ln_facet); \n\
      float g1, eval;\n\
      SMITH_G1(ld_i, ln_facet, roughness, g1);\n\
      GGX_EVAL(ln_facet, roughness, eval);\n\
      ggx_pdf = g1 * fabs(DOT(ld_i, ln_facet)) * eval / fabs(ld_i[2]);\n\
    }\n\
    if (ld_i[2] == 0.0f) {\n\
      ggx_pdf = 0.0f;\n\
    }\n\
    last_brdf_pdf = ray_state[RS_LAST_BRDF_PDF];\n\
    float coating_dropoff = fmin(smoothness * 2.0f, 1.0f);\n\
    float brdf_color[3];\n\
    float ld_o[3];\n\
    float sample_dice;\n\
    PRNG(global_id, b_seeds_0.z, sample_dice);\n\
    if (glass_material || (sample_dice >= metallic)) {\n\
      // Dielectric interface\n\
      float fresnel, cos_theta_t;\n\
      DIELECTRIC_FRESNEL(ld_i[2], 1.5f, cos_theta_t, fresnel);\n\
      float reflect_sample;\n\
      PRNG(global_id, b_seeds_0.w, reflect_sample);\n\
      if (reflect_sample <= (fresnel * coating_dropoff)) {\n\
        // Reflect\n\
        float dot_ni = DOT(ln_facet, ld_i);\n\
        ld_o[0] = 2.0f * ln_facet[0] * dot_ni - ld_i[0];\n\
        ld_o[1] = 2.0f * ln_facet[1] * dot_ni - ld_i[1];\n\
        ld_o[2] = 2.0f * ln_facet[2] * dot_ni - ld_i[2];\n\
        ld_o[2] = fabs(ld_o[2]);\n\
        NORMALIZE(ld_o);\n\
        if ((ggx_pdf == 0.0f) || (ld_o[2] <= 0.0f)) {\n\
          brdf_color[0] = 0.0f;\n\
          brdf_color[1] = 0.0f;\n\
          brdf_color[2] = 0.0f;\n\
          new_brdf_pdf = -1.0f;\n\
        } else {\n\
          brdf_color[0] = 1.0f;\n\
          brdf_color[1] = 1.0f;\n\
          brdf_color[2] = 1.0f;\n\
          /*\n\
          float weight;\n\
          SMITH_G1(ld_o, ln_facet, roughness, weight);\n\
          brdf_color[0] *= weight;\n\
          brdf_color[1] *= weight;\n\
          brdf_color[2] *= weight; */\n\
          if (last_brdf_pdf != DIRAC_PDF) {\n\
            new_brdf_pdf = -1.0f;\n\
          } else {\n\
            if (ggx_pdf == DIRAC_PDF) {\n\
              new_brdf_pdf = DIRAC_PDF;\n\
            } else {\n\
              new_brdf_pdf = ggx_pdf;\n\
              new_brdf_pdf *= fresnel * coating_dropoff;\n\
              float dwh_dwo = 1.0f / (4.0f * DOT(ld_o, ln_facet));\n\
              new_brdf_pdf *= fabs(dwh_dwo);\n\
            }\n\
          }\n\
        }\n\
      } else {\n\
        // Transmit \n\
        if (glass_material) {\n\
          // Refract\n\
          float ior = (cos_theta_t  < 0.0f) ? (1.0f / 1.5f) : 1.5f;\n\
          float ior_2 = ior * ior;\n\
          float dot_ni = DOT(ln_facet, ld_i);\n\
          float c = dot_ni * ior + cos_theta_t;\n\
          ld_o[0] = ln_facet[0] * c - ld_i[0] * ior;\n\
          ld_o[1] = ln_facet[1] * c - ld_i[1] * ior;\n\
          ld_o[2] = ln_facet[2] * c - ld_i[2] * ior;\n\
          copysign(ld_o[2], -ld_o[1]);\n\
          NORMALIZE(ld_o);\n\
          if ((cos_theta_t == 0.0f) || (ld_i[2] * ld_o[2] >= 0.0f)) {\n\
            brdf_color[0] = 0.0f;\n\
            brdf_color[1] = 0.0f;\n\
            brdf_color[2] = 0.0f;\n\
            new_brdf_pdf = -1.0f;\n\
          } else {\n\
            brdf_color[0] = albedo[0] * ior_2;\n\
            brdf_color[1] = albedo[1] * ior_2;\n\
            brdf_color[2] = albedo[2] * ior_2;\n\
            /*\n\
            float weight;\n\
            SMITH_G1(ld_o, ln_facet, roughness, weight);\n\
            brdf_color[0] *= weight;\n\
            brdf_color[1] *= weight;\n\
            brdf_color[2] *= weight; */\n\
            if (last_brdf_pdf != DIRAC_PDF) {\n\
              new_brdf_pdf = -1.0f;\n\
            } else {\n\
              if (ggx_pdf == DIRAC_PDF) {\n\
                new_brdf_pdf = DIRAC_PDF;\n\
              } else {\n\
                float dot_no = DOT(ln_facet, ld_o);\n\
                new_brdf_pdf = ggx_pdf;\n\
                float sqrt_denom = dot_ni + ior * dot_no; \n\
                float dwh_dwo = (ior_2 * dot_no) / (sqrt_denom * sqrt_denom);\n\
                new_brdf_pdf *= fabs(dwh_dwo);\n\
              }\n\
            }\n\
          }\n\
        } else {\n\
          // Diffuse \n\
          ln_facet[0] = 0.0f;\n\
          ln_facet[1] = 0.0f;\n\
          ln_facet[2] = 1.0f;\n\
          float ln_tangent[3];\n\
          float ln_binormal[3];\n\
          GET_BASIS(ln_facet, ln_tangent, ln_binormal);\n\
          float hemi_sample[2];\n\
          PRNG(global_id, b_seeds_1.x, hemi_sample[0]);\n\
          PRNG(global_id, b_seeds_1.y, hemi_sample[1]);\n\
          float hemi_d[3];\n\
          CONCENTRIC_HEMI(hemi_sample[0], hemi_sample[1], hemi_d);\n\
          ROTATE_TO_BASIS(hemi_d, ln_facet, ln_tangent, ln_binormal, ld_o);\n\
          ld_o[2] = fabs(ld_o[2]);\n\
          brdf_color[0] = albedo[0];\n\
          brdf_color[1] = albedo[1];\n\
          brdf_color[2] = albedo[2];\n\
          new_brdf_pdf = INV_PI * ld_o[2];\n\
        }\n\
      }\n\
    } else {\n\
      // Conductor\n\
      float dot_ni = DOT(ln_facet, ld_i);\n\
      ld_o[0] = 2.0f * ln_facet[0] * dot_ni - ld_i[0];\n\
      ld_o[1] = 2.0f * ln_facet[1] * dot_ni - ld_i[1];\n\
      ld_o[2] = 2.0f * ln_facet[2] * dot_ni - ld_i[2];\n\
      ld_o[2] = fabs(ld_o[2]);\n\
      NORMALIZE(ld_o);\n\
      if ((ggx_pdf == 0.0f) || (ld_o[2] <= 0.0f)) {\n\
        brdf_color[0] = 0.0f;\n\
        brdf_color[1] = 0.0f;\n\
        brdf_color[2] = 0.0f;\n\
        new_brdf_pdf = -1.0f;\n\
      } else {\n\
        float f;\n\
        CONDUCTOR_FRESNEL(dot_ni, 1.5f, 3.0f, f);\n\
        brdf_color[0] = albedo[0] * f;\n\
        brdf_color[1] = albedo[1] * f;\n\
        brdf_color[2] = albedo[2] * f;\n\
        /*\n\
        float weight;\n\
        SMITH_G1(ld_o, ln_facet, roughness, weight);\n\
        brdf_color[0] *= weight;\n\
        brdf_color[1] *= weight;\n\
        brdf_color[2] *= weight; */\n\
        if (last_brdf_pdf != DIRAC_PDF) {\n\
          new_brdf_pdf = -1.0f;\n\
        } else {\n\
          if (ggx_pdf == DIRAC_PDF) {\n\
            new_brdf_pdf = DIRAC_PDF;\n\
          } else {\n\
            new_brdf_pdf = ggx_pdf / (4.0f * DOT(ld_o, ln_facet));\n\
          }\n\
        }\n\
      }\n\
    }\n\
    float d_o[3];\n\
    ROTATE_TO_BASIS(ld_o, n, n_tangent, n_binormal, d_o);\n\
    ray_state[RS_LAST_BRDF_PDF] = new_brdf_pdf;\n\
    old_throughput[0] = ray_state[RS_THROUGHPUT_R];\n\
    old_throughput[1] = ray_state[RS_THROUGHPUT_G];\n\
    old_throughput[2] = ray_state[RS_THROUGHPUT_B];\n\
    ray_state[RS_THROUGHPUT_R] = old_throughput[0] * brdf_color[0];\n\
    ray_state[RS_THROUGHPUT_G] = old_throughput[1] * brdf_color[1];\n\
    ray_state[RS_THROUGHPUT_B] = old_throughput[2] * brdf_color[2];\n\
    ray_state[RS_DIR_X] = d_o[0];\n\
    ray_state[RS_DIR_Y] = d_o[1];\n\
    ray_state[RS_DIR_Z] = d_o[2];\n\
    ray_state[RS_STATE] = STATE_SECONDARY;\n\
  }\n\
  float sun_d[3];\n\
  sun_d[0] = FLT_MAX;\n\
  if (sample_sun > 0) {\n\
    float theta, z;\n\
    PRNG(global_id, b_seeds_1.z, theta);\n\
    PRNG(global_id, b_seeds_1.w, z);\n\
    theta *= TAU;\n\
    z = (z * (1.0f - SUN_COS_THETA)) + SUN_COS_THETA;\n\
    float z2 = native_sqrt(1.0f - (z * z));\n\
    float sun_ld_o[3];\n\
    sun_ld_o[0] = z2 * native_cos(theta);\n\
    sun_ld_o[1] = z2 * native_sin(theta);\n\
    sun_ld_o[2] = z;\n\
    float sun_tangent[3];\n\
    float sun_binormal[3];\n\
    GET_BASIS(sun.direction, sun_tangent, sun_binormal);\n\
    ROTATE_TO_BASIS(sun_ld_o, sun.direction, sun_tangent, sun_binormal, \n\
                    sun_d);\n\
    sun_d[1] = fmax(0.01f, sun_d[1]);\n\
    NORMALIZE(sun_d);\n\
    float cos_at_tri = DOT(sun_d, n);\n\
    if (cos_at_tri <= 0.0f) {\n\
      sun_d[0] = FLT_MAX;\n\
    } else {\n\
      float sun_color[3];\n\
      float brdf_color[3];\n\
      float brdf_pdf;\n\
      float sun_pdf = 1.0f / (TAU * (1.0f - SUN_COS_THETA));\n\
      float s = -6360.5f * sun_d[1];\n\
      float mu = -s * 0.00015722034f;\n\
      float nu = sun_d[0] * sun.direction[0] + sun_d[1] * sun.direction[1] + \n\
                 sun_d[2] * sun.direction[2];\n\
      float mus = (6360.5f * sun.direction[1]) * 0.00015722034f;\n\
      float rmu = 6360.5f * mu;\n\
      float delta = rmu * rmu - 6360.25f;\n\
      float4 cst;\n\
      if ((rmu < 0.0f) && (delta > 0.0f)) {\n\
        cst = (float4)(1.0f, 0.0f, 0.0f, 0.484375f);\n\
      } else {\n\
        cst = (float4)(-1.0f, 766800.000001f, 875.67117116f, 0.515625f);\n\
      }\n\
      float umu = cst.w + (rmu * cst.x + native_sqrt(delta + cst.y)) / \n\
                  (79.7511755399f + cst.z) * 0.4921875f;\n\
      float umus = 0.015625f + \n\
                   (atan(fmax(mus, -0.1975f) * 5.34962349919f) * 0.90909090f + \n\
                   0.74f) * 0.484375f;\n\
      float lerp_value = (nu + 1.0f) * 3.5f;\n\
      float unu = floor(lerp_value);\n\
      lerp_value = lerp_value - unu;\n\
			float2 uv0 = (float2)((unu + umus) * 0.125f, umu);\n\
			float2 uv1 = (float2)((unu + umus + 1.0f) * 0.125f, umu);\n\
      const sampler_t sampler = CLK_NORMALIZED_COORDS_TRUE |\n\
                                CLK_ADDRESS_REPEAT | CLK_FILTER_LINEAR;\n\
      float4 raymie = fmax((read_imagef(sun_table, sampler, uv0) * (1.0f - lerp_value)) +\n\
                           (read_imagef(sun_table, sampler, uv1) * lerp_value), 0.0f);\n\
      float pr = 0.05968310365f * (1.0f + nu * nu);\n\
      float pm = 0.04297183463f * pow(1.64f - 1.6f * nu, -1.5f) * \n\
                 (1.0f + nu * nu) * 0.378378378f;\n\
      float4 beta_r = (float4)(0.0058f, 0.0135f, 0.0331f, 1.0f);\n\
      float4 mie = raymie * raymie.w / NOT_ZERO(raymie.x) * (beta_r.x / beta_r);\n\
      float4 sun_result = fmax(raymie * pr + mie * pm, (float4)(0.0f, 0.0f, 0.0f, 0.0f)) * \n\
                          30.0f * sun.sky_intensity;\n\
      sun_result.x *= sun.color[0];\n\
      sun_result.y *= sun.color[1];\n\
      sun_result.z *= sun.color[2];\n\
      sun.sun_intensity *= 1.0f + (sun.direction[1] * 4.0f);\n\
      float horizon_falloff = fabs(sun_d[1] - 0.075f) - (sun_d[1] - 0.075f);\n\
      horizon_falloff = pow(horizon_falloff, 4.0f);\n\
      float hfo = horizon_falloff * 1000.0f;\n\
      sun.sun_intensity *= 1.0f + (sun.direction[1] * 4.0f);\n\
      float multiplier = clamp(1.0f / NOT_ZERO(hfo), 1.0f,\n\
                               fmax(5.0f, sun.sun_intensity * 100.0f));\n\
      sun_color[0] = sun_result.x * multiplier;\n\
      sun_color[1] = sun_result.y * multiplier;\n\
      sun_color[2] = sun_result.z * multiplier;\n\
      float horizon_bias = fmin(0.5f, \n\
                                (fmax(0.0f, sun.direction[1] - 0.1f) * 10.0f));\n\
      float max_component = fmax(sun_color[0], fmax(sun_color[1], sun_color[2]));\n\
      sun_color[0] = mix(sun_color[0], max_component * 1.0f, horizon_bias);\n\
      sun_color[1] = mix(sun_color[1], max_component * 0.98f, horizon_bias); \n\
      sun_color[2] = mix(sun_color[2], max_component * 0.73f, horizon_bias); \n\
      float ld_o[3];\n\
      RELATIVE_TO_BASIS(sun_d, n, n_tangent, n_binormal, ld_o);\n\
			float roughness[2] = { 1.0f - smoothness,\n\
														 1.0f - smoothness };\n\
      float ln_facet[3];\n\
      ln_facet[0] = ld_i[0] + ld_o[0];\n\
      ln_facet[1] = ld_i[1] + ld_o[1];\n\
      ln_facet[2] = ld_i[2] + ld_o[2];\n\
      NORMALIZE(ln_facet);\n\
      float coating_dropoff = fmin(smoothness * 2.0f, 1.0f);\n\
      float sample_dice;\n\
      PRNG(global_id, b_seeds_2.x, sample_dice);\n\
			if (glass_material || (sample_dice >= metallic)) {\n\
        // Dielectric interface\n\
        bool refract = (ld_i[2] * ld_o[2]) <= 0.0f;\n\
        if (refract) {\n\
          // Refraction facet\n\
          float ior = (ld_i[2] < 0.0f) ? (1.0f / 1.5f) : 1.5f;\n\
          ln_facet[0] = ld_i[0] + ld_o[0] * ior;\n\
          ln_facet[1] = ld_i[1] + ld_o[1] * ior;\n\
          ln_facet[2] = ld_i[2] + ld_o[2] * ior;\n\
          NORMALIZE(ln_facet);\n\
        }\n\
        if (ln_facet[2] < 0.0f) {\n\
          ln_facet[0] *= -1.0f;\n\
          ln_facet[1] *= -1.0f;\n\
          ln_facet[2] *= -1.0f;\n\
        }\n\
        float dot_ni = DOT(ld_i, ln_facet);\n\
        float d, gi, go, g;\n\
        GGX_EVAL(ln_facet, roughness, d);\n\
        float cos_theta_t, fresnel;\n\
        DIELECTRIC_FRESNEL(dot_ni, 1.5f, cos_theta_t, fresnel);\n\
        SMITH_G1(ld_i, ln_facet, roughness, gi);\n\
        SMITH_G1(ld_o, ln_facet, roughness, go);\n\
        g = gi * go;\n\
        if (!refract) {\n\
          // Reflect/transmit to diffuse.\n\
          float reflect_sample;\n\
          PRNG(global_id, b_seeds_2.y, reflect_sample);\n\
          if (glass_material || (reflect_sample <= (fresnel * coating_dropoff))) {\n\
            // Reflection\n\
            if ((last_brdf_pdf == DIRAC_PDF) && (smoothness < 1.0f)) {\n\
              if ((d == 0.0f) || (ld_i[2] == 0.0f)) {\n\
                sun_d[0] = FLT_MAX;\n\
                brdf_pdf = DIRAC_PDF;\n\
              } else {\n\
                float model = fresnel * coating_dropoff * d * g / (4.0f * fabs(ld_i[2]));\n\
                brdf_color[0] = model;\n\
                brdf_color[1] = model;\n\
                brdf_color[2] = model;\n\
                float prob;\n\
                prob = gi * fabs(dot_ni) * d / fabs(ld_i[2]);\n\
                prob *= fresnel * coating_dropoff;\n\
                float dwh_dwo = 1.0f / (4.0f * DOT(ld_o, ln_facet));\n\
                brdf_pdf = fabs(prob * dwh_dwo);\n\
              }\n\
            } else {\n\
              sun_d[0] = FLT_MAX;\n\
              brdf_pdf = DIRAC_PDF;\n\
            }\n\
          } else {\n\
            // Transmit to diffuse\n\
            brdf_color[0] = albedo[0];\n\
            brdf_color[1] = albedo[1];\n\
            brdf_color[2] = albedo[2];\n\
            brdf_pdf = INV_PI * ld_o[2];\n\
          }\n\
        } else {\n\
          // Refraction\n\
          if ((last_brdf_pdf == DIRAC_PDF) && (smoothness < 1.0f)) {\n\
            if ((d == 0.0f) || (ld_i[2] == 0.0f)) {\n\
              sun_d[0] = FLT_MAX;\n\
              brdf_pdf = DIRAC_PDF;\n\
            } else {\n\
              float ior = (ld_i[2] < 0.0f) ? (1.0f / 1.5f) : 1.5f;\n\
              float dot_no = DOT(ln_facet, ld_o);\n\
              float sqrt_denom = dot_ni + ior * dot_no; \n\
              float model = ((1.0f - fresnel) * d * g * ior * ior * dot_ni * dot_no) /\n\
                            (ld_i[2] * sqrt_denom * sqrt_denom);\n\
              model *= (ld_i[2] < 0.0f) ? 1.5f : (1.0f / 1.5f);\n\
              brdf_color[0] = albedo[0] * model;\n\
              brdf_color[1] = albedo[1] * model;\n\
              brdf_color[2] = albedo[2] * model;\n\
              float prob;\n\
              prob = gi * fabs(dot_ni) * d / fabs(ld_i[2]);\n\
              prob *= fresnel * coating_dropoff;\n\
              float dwh_dwo = (ior * ior * dot_no) / (sqrt_denom * sqrt_denom); \n\
              brdf_pdf = fabs(prob * dwh_dwo);\n\
            }\n\
          } else {\n\
            sun_d[0] = FLT_MAX;\n\
            brdf_pdf = DIRAC_PDF;\n\
          }\n\
        }\n\
      } else {\n\
        // Conductor\n\
        if ((last_brdf_pdf == DIRAC_PDF) && (smoothness < 1.0f)) {\n\
          float dot_ni = DOT(ln_facet, ld_i);\n\
          float d, f, g, gi, go;\n\
          GGX_EVAL(ln_facet, roughness, d);\n\
          CONDUCTOR_FRESNEL(dot_ni, 1.5f, 3.0f, f);\n\
          SMITH_G1(ld_i, ln_facet, roughness, gi);\n\
          SMITH_G1(ld_o, ln_facet, roughness, go);\n\
          g = gi * go;\n\
          float model = f * (d * g / (4.0f * ld_i[2]));\n\
          brdf_color[0] = albedo[0] * model;\n\
          brdf_color[1] = albedo[1] * model;\n\
          brdf_color[2] = albedo[2] * model;\n\
          brdf_pdf = d * gi / (4.0f * ld_i[2]);\n\
        } else {\n\
          sun_d[0] = FLT_MAX;\n\
          brdf_pdf = DIRAC_PDF;\n\
        }\n\
      }\n\
      float mis = MIS(sun_pdf, brdf_pdf);\n\
      ray_state[RS_SUN_R] = old_throughput[0] * (sun_color[0] / NOT_ZERO(sun_pdf)) *\n\
                            brdf_color[0] * mis;\n\
      ray_state[RS_SUN_G] = old_throughput[1] * (sun_color[1] / NOT_ZERO(sun_pdf)) *\n\
                            brdf_color[1] * mis;\n\
      ray_state[RS_SUN_B] = old_throughput[2] * (sun_color[2] / NOT_ZERO(sun_pdf)) *\n\
                            brdf_color[2] * mis;\n\
    }\n\
  }\n\
  float o[3];\n\
  o[0] = ray_state[RS_ORIGIN_X];\n\
  o[1] = ray_state[RS_ORIGIN_Y];\n\
  o[2] = ray_state[RS_ORIGIN_Z];\n\
  float emitter_d[3];\n\
  emitter_d[0] = FLT_MAX;\n\
  if (emitter_count > 0) {\n\
    float emitter_uv_sample[2];\n\
    PRNG(global_id, b_seeds_2.z, emitter_uv_sample[0]);\n\
    PRNG(global_id, b_seeds_2.w, emitter_uv_sample[1]);\n\
    if ((emitter_uv_sample[0] + emitter_uv_sample[1]) >= 1.0f) {\n\
      emitter_uv_sample[0] = 1.0f - emitter_uv_sample[0]; \n\
      emitter_uv_sample[1] = 1.0f - emitter_uv_sample[1]; \n\
    }\n\
    float emit_u = emitter_uv_sample[0];\n\
    float emit_v = emitter_uv_sample[1];\n\
    float emit_w = 1.0f - emit_u - emit_v;\n\
    float emitter_sample;\n\
    PRNG(global_id, b_seeds_3.x, emitter_sample);\n\
    uint emitter_choice = emitter_sample * emitter_count;\n\
    EmitterIndex emitter_index = emitters[emitter_choice];\n\
    uint emitter_tri_index = emitter_index.mesh_triangle_offset + \n\
                                 emitter_index.triangle_offset;\n\
    int tri_pixel = emitter_tri_index * 42;\n\
    float emitter_position[3];\n\
    if (use_triangles_image) {\n\
      emitter_position[0] = read_imagef(triangles_image, TRI_VERTEX).x +\n\
        (emit_u * read_imagef(triangles_image, TRI_EDGES_0).x) +\n\
        (emit_v * -read_imagef(triangles_image, TRI_EDGES_1).x);\n\
      emitter_position[1] = read_imagef(triangles_image, TRI_VERTEX + 1).x +\n\
        (emit_u * read_imagef(triangles_image, TRI_EDGES_0 + 1).x) +\n\
        (emit_v * -read_imagef(triangles_image, TRI_EDGES_1 + 1).x);\n\
      emitter_position[2] = read_imagef(triangles_image, TRI_VERTEX + 2).x +\n\
        (emit_u * read_imagef(triangles_image, TRI_EDGES_0 + 2).x) +\n\
        (emit_v * -read_imagef(triangles_image, TRI_EDGES_1 + 2).x);\n\
    } else {\n\
      emitter_position[0] = triangles[emitter_tri_index].vertex[0] +\n\
        (emit_u * triangles[emitter_tri_index].edges[0][0]) +\n\
        (emit_v * -triangles[emitter_tri_index].edges[1][0]);\n\
      emitter_position[1] = triangles[emitter_tri_index].vertex[1] +\n\
        (emit_u * triangles[emitter_tri_index].edges[0][1]) +\n\
        (emit_v * -triangles[emitter_tri_index].edges[1][1]);\n\
      emitter_position[2] = triangles[emitter_tri_index].vertex[2] +\n\
        (emit_u * triangles[emitter_tri_index].edges[0][2]) +\n\
        (emit_v * -triangles[emitter_tri_index].edges[1][2]);\n\
    }\n\
    emitter_d[0] = emitter_position[0] - o[0];\n\
    emitter_d[1] = emitter_position[1] - o[1];\n\
    emitter_d[2] = emitter_position[2] - o[2];\n\
    float t_2 = DOT(emitter_d, emitter_d);\n\
    float t = native_sqrt(t_2);\n\
    ray_state[RS_EMITTER_T] = t;\n\
    float inv_t = 1.0f / t;\n\
    emitter_d[0] *= inv_t;\n\
    emitter_d[1] *= inv_t;\n\
    emitter_d[2] *= inv_t;\n\
    float emitter_n[3];\n\
    float inverse_area;\n\
    uint mat_id;\n\
    if (use_triangles_image) {\n\
      mat_id = read_imageui(triangles_image, TRI_MAT_ID).x;\n\
      inverse_area = read_imagef(triangles_image, TRI_INV_AREA).x;\n\
      emitter_n[0] = emit_w * read_imagef(triangles_image, TRI_VN_0).x + \n\
                     emit_u * read_imagef(triangles_image, TRI_VN_1).x +  \n\
                     emit_v * read_imagef(triangles_image, TRI_VN_2).x;\n\
      emitter_n[1] = emit_w * read_imagef(triangles_image, TRI_VN_0 + 1).x + \n\
                     emit_u * read_imagef(triangles_image, TRI_VN_1 + 1).x +  \n\
                     emit_v * read_imagef(triangles_image, TRI_VN_2 + 1).x;\n\
      emitter_n[2] = emit_w * read_imagef(triangles_image, TRI_VN_0 + 2).x + \n\
                     emit_u * read_imagef(triangles_image, TRI_VN_1 + 2).x +  \n\
                     emit_v * read_imagef(triangles_image, TRI_VN_2 + 2).x;\n\
    } else {\n\
      mat_id = triangles[emitter_tri_index].material_id; \n\
      inverse_area = triangles[emitter_tri_index].inverse_area; \n\
      emitter_n[0] = emit_w * triangles[emitter_tri_index].vertex_normals[0][0] + \n\
                     emit_u * triangles[emitter_tri_index].vertex_normals[1][0] + \n\
                     emit_v * triangles[emitter_tri_index].vertex_normals[2][0];\n\
      emitter_n[1] = emit_w * triangles[emitter_tri_index].vertex_normals[0][1] + \n\
                     emit_u * triangles[emitter_tri_index].vertex_normals[1][1] + \n\
                     emit_v * triangles[emitter_tri_index].vertex_normals[2][1];\n\
      emitter_n[2] = emit_w * triangles[emitter_tri_index].vertex_normals[0][2] + \n\
                     emit_u * triangles[emitter_tri_index].vertex_normals[1][2] + \n\
                     emit_v * triangles[emitter_tri_index].vertex_normals[2][2];\n\
    }\n\
    NORMALIZE(emitter_n);\n\
    float cos_at_emitter = DOT(-emitter_d, emitter_n);\n\
    float cos_at_tri = DOT(emitter_d, n);\n\
    if ((cos_at_emitter <= 0.0f) || (cos_at_tri <= 0.0f) || \n\
        (t_2 < (HI_EPSILON * 4.0f))) {\n\
      emitter_d[0] = FLT_MAX; \n\
    } else {\n\
      float brdf_color[3];\n\
      float brdf_pdf;\n\
      float emitter_prob = 1.0f / (float)emitter_count;\n\
      float emitter_pdf = inverse_area * (t_2 / fabs(cos_at_emitter)) * emitter_prob;\n\
      float intensity = materials[mat_id].emission * 25.0f;\n\
      float emitter_color[3];\n\
      float uv[2];\n\
      if (materials[mat_id].map_flags & MAP_FLAG_EMISSION) {\n\
        float2 emission_uv;\n\
        if (use_triangles_image) {\n\
          emission_uv.x = emit_w * read_imagef(triangles_image, TRI_UV_0).x +\n\
                          emit_u * read_imagef(triangles_image, TRI_UV_1).x +\n\
                          emit_v * read_imagef(triangles_image, TRI_UV_2).x;\n\
          emission_uv.y = emit_w * read_imagef(triangles_image, TRI_UV_0 + 1).x +\n\
                          emit_u * read_imagef(triangles_image, TRI_UV_1 + 1).x +\n\
                          emit_v * read_imagef(triangles_image, TRI_UV_2 + 1).x;\n\
        } else {\n\
          emission_uv.x = emit_w * triangles[emitter_tri_index].vertex_uvs[0][0] +\n\
                          emit_u * triangles[emitter_tri_index].vertex_uvs[1][0] +\n\
                          emit_v * triangles[emitter_tri_index].vertex_uvs[2][0];\n\
          emission_uv.y = emit_w * triangles[emitter_tri_index].vertex_uvs[0][1] +\n\
                          emit_u * triangles[emitter_tri_index].vertex_uvs[1][1] +\n\
                          emit_v * triangles[emitter_tri_index].vertex_uvs[2][1];\n\
        }\n\
        emission_uv.x = mix(materials[mat_id].emission_bounds[0] + 0.5f,\n\
                             materials[mat_id].emission_bounds[1] - 0.5f, \n\
                             emission_uv.x);\n\
        emission_uv.y = mix(materials[mat_id].emission_bounds[2] + 0.5f,\n\
                             materials[mat_id].emission_bounds[3] - 0.5f, \n\
                             emission_uv.y);\n\
        const sampler_t sampler = CLK_NORMALIZED_COORDS_TRUE |\n\
                                  CLK_ADDRESS_REPEAT | CLK_FILTER_LINEAR;\n\
        float4 emission_map = read_imagef(emission_atlas, sampler, emission_uv);\n\
        emitter_color[0] = emission_map.x * intensity;\n\
        emitter_color[1] = emission_map.y * intensity;\n\
        emitter_color[2] = emission_map.z * intensity;\n\
      } else {\n\
        emitter_color[0] = materials[mat_id].albedo[0] * intensity;\n\
        emitter_color[1] = materials[mat_id].albedo[1] * intensity;\n\
        emitter_color[2] = materials[mat_id].albedo[2] * intensity;\n\
      }\n\
      if (fmax(fmax(emitter_color[0], emitter_color[1]), emitter_color[2]) > 0.0f) {\n\
        emitter_color[0] /= emitter_pdf;\n\
        emitter_color[1] /= emitter_pdf;\n\
        emitter_color[2] /= emitter_pdf;\n\
        float ld_o[3];\n\
        RELATIVE_TO_BASIS(emitter_d, n, n_tangent, n_binormal, ld_o);\n\
        float roughness[2] = { 1.0f - smoothness,\n\
                               1.0f - smoothness };\n\
        float ln_facet[3];\n\
        ln_facet[0] = ld_i[0] + ld_o[0];\n\
        ln_facet[1] = ld_i[1] + ld_o[1];\n\
        ln_facet[2] = ld_i[2] + ld_o[2];\n\
        NORMALIZE(ln_facet);\n\
        float coating_dropoff = fmin(smoothness * 2.0f, 1.0f);\n\
        float sample_dice;\n\
        PRNG(global_id, b_seeds_2.x, sample_dice);\n\
        if (glass_material || (sample_dice >= metallic)) {\n\
          // Dielectric interface\n\
          bool refract = (ld_i[2] * ld_o[2]) <= 0.0f;\n\
          if (refract) {\n\
            // Refraction facet\n\
            float ior = (ld_i[2] < 0.0f) ? (1.0f / 1.5f) : 1.5f;\n\
            ln_facet[0] = ld_i[0] + ld_o[0] * ior;\n\
            ln_facet[1] = ld_i[1] + ld_o[1] * ior;\n\
            ln_facet[2] = ld_i[2] + ld_o[2] * ior;\n\
            NORMALIZE(ln_facet);\n\
          }\n\
          if (ln_facet[2] < 0.0f) {\n\
            ln_facet[0] *= -1.0f;\n\
            ln_facet[1] *= -1.0f;\n\
            ln_facet[2] *= -1.0f;\n\
          }\n\
          float dot_ni = DOT(ld_i, ln_facet);\n\
          float d, gi, go, g;\n\
          GGX_EVAL(ln_facet, roughness, d);\n\
          float cos_theta_t, fresnel;\n\
          DIELECTRIC_FRESNEL(dot_ni, 1.5f, cos_theta_t, fresnel);\n\
          SMITH_G1(ld_i, ln_facet, roughness, gi);\n\
          SMITH_G1(ld_o, ln_facet, roughness, go);\n\
          g = gi * go;\n\
          if (!refract) {\n\
            // Reflect/transmit to diffuse.\n\
            float reflect_sample;\n\
            PRNG(global_id, b_seeds_2.y, reflect_sample);\n\
            if (glass_material || (reflect_sample <= (fresnel * coating_dropoff))) {\n\
              // Reflection\n\
              if ((last_brdf_pdf == DIRAC_PDF) && (smoothness < 1.0f)) {\n\
                if ((d == 0.0f) || (ld_i[2] == 0.0f)) {\n\
                  emitter_d[0] = FLT_MAX;\n\
                  brdf_pdf = DIRAC_PDF;\n\
                } else {\n\
                  float model = fresnel * coating_dropoff * d * g / (4.0f * fabs(ld_i[2]));\n\
                  brdf_color[0] = model;\n\
                  brdf_color[1] = model;\n\
                  brdf_color[2] = model;\n\
                  float prob;\n\
                  prob = gi * fabs(dot_ni) * d / fabs(ld_i[2]);\n\
                  prob *= fresnel * coating_dropoff;\n\
                  float dwh_dwo = 1.0f / (4.0f * DOT(ld_o, ln_facet));\n\
                  brdf_pdf = fabs(prob * dwh_dwo);\n\
                }\n\
              } else {\n\
                emitter_d[0] = FLT_MAX;\n\
              }\n\
            } else {\n\
              // Transmit to diffuse\n\
              brdf_color[0] = albedo[0];\n\
              brdf_color[1] = albedo[1];\n\
              brdf_color[2] = albedo[2];\n\
              brdf_pdf = INV_PI * ld_o[2];\n\
            }\n\
          } else {\n\
            // Refraction\n\
            if ((last_brdf_pdf == DIRAC_PDF) && (smoothness < 1.0f)) {\n\
              if ((d == 0.0f) || (ld_i[2] == 0.0f)) {\n\
                emitter_d[0] = FLT_MAX;\n\
                brdf_pdf = DIRAC_PDF;\n\
              } else {\n\
                float ior = (ld_i[2] < 0.0f) ? (1.0f / 1.5f) : 1.5f;\n\
                float dot_no = DOT(ln_facet, ld_o);\n\
                float sqrt_denom = dot_ni + ior * dot_no; \n\
                float model = ((1.0f - fresnel) * d * g * ior * ior * dot_ni * dot_no) /\n\
                              (ld_i[2] * sqrt_denom * sqrt_denom);\n\
                model *= (ld_i[2] < 0.0f) ? 1.5f : (1.0f / 1.5f);\n\
                brdf_color[0] = albedo[0] * model;\n\
                brdf_color[1] = albedo[1] * model;\n\
                brdf_color[2] = albedo[2] * model;\n\
                float prob;\n\
                prob = gi * fabs(dot_ni) * d / fabs(ld_i[2]);\n\
                prob *= fresnel * coating_dropoff;\n\
                float dwh_dwo = (ior * ior * dot_no) / (sqrt_denom * sqrt_denom); \n\
                brdf_pdf = fabs(prob * dwh_dwo);\n\
              }\n\
            } else {\n\
              emitter_d[0] = FLT_MAX;\n\
            }\n\
          }\n\
        } else {\n\
          // Conductor\n\
          if ((last_brdf_pdf == DIRAC_PDF) && (smoothness < 1.0f)) {\n\
            float dot_ni = DOT(ln_facet, ld_i);\n\
            float d, f, g, gi, go;\n\
            GGX_EVAL(ln_facet, roughness, d);\n\
            CONDUCTOR_FRESNEL(dot_ni, 1.5f, 3.0f, f);\n\
            SMITH_G1(ld_i, ln_facet, roughness, gi);\n\
            SMITH_G1(ld_o, ln_facet, roughness, go);\n\
            g = gi * go;\n\
            float model = f * (d * g / (4.0f * ld_i[2]));\n\
            brdf_color[0] = albedo[0] * model;\n\
            brdf_color[1] = albedo[1] * model;\n\
            brdf_color[2] = albedo[2] * model;\n\
            brdf_pdf = d * gi / (4.0f * ld_i[2]);\n\
          } else {\n\
            emitter_d[0] = FLT_MAX;\n\
          }\n\
        }\n\
      } else {\n\
        emitter_d[0] = FLT_MAX;\n\
      }\n\
      float mis = MIS(emitter_pdf, brdf_pdf);\n\
      ray_state[RS_EMITTER_R] = old_throughput[0] * emitter_color[0] * \n\
                                brdf_color[0] * mis;\n\
      ray_state[RS_EMITTER_G] = old_throughput[1] * emitter_color[1] * \n\
                                brdf_color[1] * mis;\n\
      ray_state[RS_EMITTER_B] = old_throughput[2] * emitter_color[2] * \n\
                                brdf_color[2] * mis;\n\
    }\n\
  }\n\
  ray_state[RS_SUN_D_X] = sun_d[0];\n\
  ray_state[RS_SUN_D_Y] = sun_d[1];\n\
  ray_state[RS_SUN_D_Z] = sun_d[2];\n\
  ray_state[RS_EMITTER_D_X] = emitter_d[0];\n\
  ray_state[RS_EMITTER_D_Y] = emitter_d[1];\n\
  ray_state[RS_EMITTER_D_Z] = emitter_d[2];\n\
}\n\
///\n\
kernel void NEE(global float* ray_state, \n\
                global Material* materials,\n\
                int use_nodes_image,\n\
                read_only image1d_buffer_t nodes_image,\n\
                global TwoNode* nodes,\n\
                int top_tree_offset,\n\
                int use_triangles_image,\n\
                read_only image1d_buffer_t triangles_image,\n\
                global TreeTriangle* triangles,\n\
                read_only image2d_t albedo_atlas,\n\
                read_only image2d_t emission_atlas,\n\
                uint4  b_seeds,\n\
                int width, \n\
                int height)\n\
{\n\
  int global_id = get_global_id(0); \n\
  int ray_count = width * height;\n\
  if (global_id >= ray_count) {\n\
    return;\n\
  }\n\
  int state = ray_state[RS_STATE];\n\
  if (state == STATE_END) {\n\
    return;\n\
  }\n\
  float o[3];\n\
  o[0] = ray_state[RS_ORIGIN_X];\n\
  o[1] = ray_state[RS_ORIGIN_Y];\n\
  o[2] = ray_state[RS_ORIGIN_Z];\n\
  float sun_d[3];\n\
  sun_d[0] = ray_state[RS_SUN_D_X];\n\
  if (sun_d[0] < FLT_MAX) {\n\
    sun_d[1] = ray_state[RS_SUN_D_Y];\n\
    sun_d[2] = ray_state[RS_SUN_D_Z];\n\
    float sun_o[3];\n\
    sun_o[0] = o[0] + (sun_d[0] * EPSILON);\n\
    sun_o[1] = o[1] + (sun_d[1] * EPSILON);\n\
    sun_o[2] = o[2] + (sun_d[2] * EPSILON);\n\
    float h_t = FLT_MAX;\n\
    OCCLUSION(sun_o, sun_d, use_nodes_image, use_triangles_image);\n\
    if (h_t == FLT_MAX) {\n\
      ray_state[RS_COLOR_R] += ray_state[RS_SUN_R];\n\
      ray_state[RS_COLOR_G] += ray_state[RS_SUN_G];\n\
      ray_state[RS_COLOR_B] += ray_state[RS_SUN_B];\n\
    }  \n\
  }\n\
  float emitter_d[3];\n\
  emitter_d[0] = ray_state[RS_EMITTER_D_X];\n\
  if (emitter_d[0] < FLT_MAX) {\n\
    emitter_d[1] = ray_state[RS_EMITTER_D_Y];\n\
    emitter_d[2] = ray_state[RS_EMITTER_D_Z];\n\
    float emitter_o[3];\n\
    emitter_o[0] = o[0] + (emitter_d[0] * EPSILON);\n\
    emitter_o[1] = o[1] + (emitter_d[1] * EPSILON);\n\
    emitter_o[2] = o[2] + (emitter_d[2] * EPSILON);\n\
    float emitter_t = ray_state[RS_EMITTER_T] - (2.0f * EPSILON);\n\
    float h_t = emitter_t;\n\
    OCCLUSION(emitter_o, emitter_d, use_nodes_image, use_triangles_image);\n\
    if (h_t == emitter_t) {\n\
      ray_state[RS_COLOR_R] += ray_state[RS_EMITTER_R];\n\
      ray_state[RS_COLOR_G] += ray_state[RS_EMITTER_G];\n\
      ray_state[RS_COLOR_B] += ray_state[RS_EMITTER_B];\n\
    }  \n\
  }\n\
  float throughput[3];\n\
  throughput[0] = ray_state[RS_THROUGHPUT_R];\n\
  throughput[1] = ray_state[RS_THROUGHPUT_G];\n\
  throughput[2] = ray_state[RS_THROUGHPUT_B];\n\
  float tp = fmax(fmax(throughput[0], throughput[1]), throughput[2]);\n\
  if (tp <= THROUGHPUT_THRESHOLD) {\n\
    ray_state[RS_STATE] = STATE_END;\n\
  } else {\n\
    float roulette_prob = tp;\n\
    if (roulette_prob <= 1.0f) {\n\
      float roulette;\n\
      PRNG(global_id, b_seeds.z, roulette);\n\
      if (roulette < roulette_prob) {\n\
        float modify_throughput = 1.0f / roulette_prob;\n\
        ray_state[RS_THROUGHPUT_R] = throughput[0] * modify_throughput;\n\
        ray_state[RS_THROUGHPUT_G] = throughput[1] * modify_throughput;\n\
        ray_state[RS_THROUGHPUT_B] = throughput[2] * modify_throughput;\n\
      } else {\n\
        ray_state[RS_STATE] = STATE_END;\n\
      }\n\
    }\n\
  }\n\
}\n\
///\n\
kernel void Accumulate(global float* ray_state,\n\
                       write_only image2d_t target,\n\
                       ImageProperties image_properties,\n\
                       int width,\n\
                       int height,\n\
                       float inv_sample_count)\n\
{\n\
  int global_id = get_global_id(1) * get_global_size(0) + get_global_id(0);\n\
  int ray_count = width * height;\n\
  if (global_id >= ray_count) {\n\
    return;\n\
  }\n\
  float rgb[3];\n\
	rgb[0] = ray_state[RS_COLOR_R];\n\
	rgb[1] = ray_state[RS_COLOR_G];\n\
	rgb[2] = ray_state[RS_COLOR_B];\n\
	AVG_TONEMAP(rgb);\n\
  float lum_coeff[3];\n\
	lum_coeff[0] = 0.2125f;\n\
	lum_coeff[1] = 0.7154f;\n\
	lum_coeff[2] = 0.0721f;\n\
	float bright_rgb[3];\n\
	bright_rgb[0] = rgb[0] * image_properties.brightness;\n\
	bright_rgb[1] = rgb[1] * image_properties.brightness;\n\
	bright_rgb[2] = rgb[2] * image_properties.brightness;\n\
	float intensity = DOT(bright_rgb, lum_coeff);\n\
	float sat_color[3];\n\
	sat_color[0] = mix(intensity, bright_rgb[0], image_properties.saturation);\n\
	sat_color[1] = mix(intensity, bright_rgb[1], image_properties.saturation);\n\
	sat_color[2] = mix(intensity, bright_rgb[2], image_properties.saturation);\n\
  rgb[0] = mix(0.5f, sat_color[0], image_properties.contrast);\n\
	rgb[1] = mix(0.5f, sat_color[1], image_properties.contrast);\n\
	rgb[2] = mix(0.5f, sat_color[2], image_properties.contrast);\n\
  float4 target_color = (float4)(rgb[0], rgb[1], rgb[2], 1.0f);\n\
  target_color = clamp(target_color, 0.0f, 1.0f);\n\
  int2 xy = (int2)(global_id % width, global_id / width);\n\
  write_imagef(target, xy, target_color);\n\
}\n\
///\n\
kernel void CompositeTwo(read_only image2d_t compositing_target_one,\n\
                         read_only image2d_t compositing_target_two,\n\
                         write_only image2d_t target,\n\
                         int width,\n\
                         int height)\n\
{\n\
  int global_id = get_global_id(1) * get_global_size(0) + get_global_id(0);\n\
  int ray_count = width * height;\n\
  if (global_id >= ray_count) {\n\
    return;\n\
  }\n\
  const sampler_t sampler = CLK_NORMALIZED_COORDS_TRUE |\n\
                            CLK_ADDRESS_CLAMP | CLK_FILTER_NEAREST;\n\
  int2 xy = (int2)(global_id % width, global_id / width);\n\
  float4 c0 = read_imagef(compositing_target_one, sampler, xy);\n\
  float4 c1 = read_imagef(compositing_target_two, sampler, xy);\n\
  float4 target_color = (c0 + c1) * 0.5f;\n\
  write_imagef(target, xy, target_color);\n\
}\n\
///\n\
kernel void CompositeThree(read_only image2d_t compositing_target_one,\n\
                           read_only image2d_t compositing_target_two,\n\
                           read_only image2d_t compositing_target_three,\n\
                           write_only image2d_t target,\n\
                           int width,\n\
                           int height)\n\
{\n\
  int global_id = get_global_id(1) * get_global_size(0) + get_global_id(0);\n\
  int ray_count = width * height;\n\
  if (global_id >= ray_count) {\n\
    return;\n\
  }\n\
  int2 xy = (int2)(global_id % width, global_id / width);\n\
  const sampler_t sampler = CLK_NORMALIZED_COORDS_TRUE |\n\
                            CLK_ADDRESS_CLAMP | CLK_FILTER_NEAREST;\n\
  float4 c0 = read_imagef(compositing_target_one, sampler, xy);\n\
  float4 c1 = read_imagef(compositing_target_two, sampler, xy);\n\
  float4 c2 = read_imagef(compositing_target_three, sampler, xy);\n\
  float4 target_color = (c0 + c1 + c2) * 0.3333333f;\n\
  write_imagef(target, xy, target_color);\n\
}\n\
///\n\
kernel void CompositeFour(read_only image2d_t compositing_target_one,\n\
                          read_only image2d_t compositing_target_two,\n\
                          read_only image2d_t compositing_target_three,\n\
                          read_only image2d_t compositing_target_four,\n\
                          write_only image2d_t target,\n\
                          int width,\n\
                          int height)\n\
{\n\
  int global_id = get_global_id(1) * get_global_size(0) + get_global_id(0);\n\
  int ray_count = width * height;\n\
  if (global_id >= ray_count) {\n\
    return;\n\
  }\n\
  int2 xy = (int2)(global_id % width, global_id / width);\n\
  const sampler_t sampler = CLK_NORMALIZED_COORDS_TRUE |\n\
                            CLK_ADDRESS_CLAMP | CLK_FILTER_NEAREST;\n\
  float4 c0 = read_imagef(compositing_target_one, sampler, xy);\n\
  float4 c1 = read_imagef(compositing_target_two, sampler, xy);\n\
  float4 c2 = read_imagef(compositing_target_three, sampler, xy);\n\
  float4 c3 = read_imagef(compositing_target_four, sampler, xy);\n\
  float4 target_color = (c0 + c1 + c2 + c3) * 0.25f;\n\
  write_imagef(target, xy, target_color);\n\
}\n\
///\n\
kernel void CompositeFive(read_only image2d_t compositing_target_one,\n\
                          read_only image2d_t compositing_target_two,\n\
                          read_only image2d_t compositing_target_three,\n\
                          read_only image2d_t compositing_target_four,\n\
                          read_only image2d_t compositing_target_five,\n\
                          write_only image2d_t target,\n\
                          int width,\n\
                          int height)\n\
{\n\
  int global_id = get_global_id(1) * get_global_size(0) + get_global_id(0);\n\
  int ray_count = width * height;\n\
  if (global_id >= ray_count) {\n\
    return;\n\
  }\n\
  int2 xy = (int2)(global_id % width, global_id / width);\n\
  const sampler_t sampler = CLK_NORMALIZED_COORDS_TRUE |\n\
                            CLK_ADDRESS_CLAMP | CLK_FILTER_NEAREST;\n\
  float4 c0 = read_imagef(compositing_target_one, sampler, xy);\n\
  float4 c1 = read_imagef(compositing_target_two, sampler, xy);\n\
  float4 c2 = read_imagef(compositing_target_three, sampler, xy);\n\
  float4 c3 = read_imagef(compositing_target_four, sampler, xy);\n\
  float4 c4 = read_imagef(compositing_target_five, sampler, xy);\n\
  float4 target_color = (c0 + c1 + c2 + c3 + c4) * 0.2f;\n\
  write_imagef(target, xy, target_color);\n\
}";
