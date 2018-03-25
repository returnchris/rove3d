using UnityEngine;
using System.Collections;
using System.Collections.Generic;
///
public class RoveObject : MonoBehaviour 
{
  public uint m_mesh_id;
  public List<RoveMaterial> m_materials;
  public uint[] m_material_indices;
  public MeshFilter m_mesh_filter;
  public MeshRenderer m_mesh_renderer;
  private RoveSetup m_rove_setup;
  private RoveMaterials m_rove_materials;
  ///
	void Start() 
  {
    m_mesh_id = RoveSetup.ERROR;
    m_rove_setup = RoveSetup.GetAPIComponent<RoveSetup>();
    if (!m_rove_setup || !m_rove_setup.enabled) {
      Debug.LogError("Rove: Could not find an active RoveSetup component.");
      Destroy(this);
      return;
    }
    m_rove_materials = RoveSetup.GetAPIComponent<RoveMaterials>();
    if (!m_rove_materials || !m_rove_materials.enabled) {
      Debug.LogError("Rove: Could not find an active RoveMaterials component.");
      Destroy(this);
      return;
    }
    m_mesh_renderer = this.GetComponent<MeshRenderer>();
    if (!m_mesh_renderer) {
      Debug.LogError("Rove: Could not find a mesh renderer on object.");
      Destroy(this);
      return;
    }
    m_mesh_filter = this.GetComponent<MeshFilter>();
    if (!m_mesh_filter) {
      Debug.LogError("Rove: Could not find a mesh filter on object.");
      Destroy(this);
      return;
    }
    gameObject.transform.hasChanged = true;
    m_mesh_renderer.enabled = false;
	}
  ///
  void Update()
  {
    if (!m_rove_setup.m_setup_success) {
      return;
    }
    if (m_mesh_id == RoveSetup.ERROR) {
      int sub_mesh_count = m_mesh_filter.sharedMesh.subMeshCount;
      m_material_indices = new uint[sub_mesh_count];
      if (m_materials.Count < sub_mesh_count) {
        m_materials = new List<RoveMaterial>(sub_mesh_count);
      }
      for (int m = 0; m < sub_mesh_count; ++m) {
        uint material_index = 0;
        if ((m_materials.Count != 0) && (m_materials[m] != null)) {
          material_index = m_rove_materials.AddMaterial(m_materials[m]);
          if (material_index == RoveSetup.ERROR) {
            m_rove_setup.m_setup_success = false;
            Destroy(m_rove_setup);
            return;
          }
        } else {
          material_index = 0;
        }
        m_material_indices[m] = material_index;
      }
      m_mesh_id = ImportMesh();
      // If you do not plan to switch between raster/Rove, uncomment this.
      // Resources.UnloadAsset(m_mesh_filter.sharedMesh);
      if (m_mesh_id == RoveSetup.ERROR) {
        m_rove_setup.m_setup_success = false;
        Destroy(m_rove_setup);
        return;
      }
    }
    if (gameObject.transform.hasChanged) {
      float[] mesh_transform = new float[16];
      Matrix4x4 mesh_mat = gameObject.transform.localToWorldMatrix;
      for (int e = 0; e < 16; ++e) {
        mesh_transform[e] = mesh_mat[e];
      }
      SetMeshTransform(mesh_transform);
      gameObject.transform.hasChanged = false;
    }
  }
  ///
  void OnEnable()
  {
    gameObject.transform.hasChanged = true;
  }
  ///
  void OnDisable()
  {
    if (!m_rove_setup.m_setup_success) {
      return;
    }
    if (m_mesh_id != RoveSetup.ERROR) {
      FreeMesh();
      m_mesh_id = RoveSetup.ERROR;
    }
  }
  ///
  void OnDestroy() 
  {
    if (!m_rove_setup) {
      return;
    }
    if (!m_rove_setup.m_setup_success) {
      return;
    }
    if (m_mesh_id != RoveSetup.ERROR) {
      FreeMesh();
      m_mesh_id = RoveSetup.ERROR;
    }
  }
  /// API functions.
  ///
  public uint ImportMesh()
  {
    Mesh mesh = m_mesh_filter.sharedMesh;
    bool has_normals = (mesh.normals.Length == mesh.vertices.Length);
    bool has_tangents = (mesh.tangents.Length == mesh.vertices.Length);
    bool has_uvs = (mesh.uv.Length == mesh.vertices.Length);
    uint mesh_id = RoveInternal.ImportMesh((uint)mesh.vertices.Length,
                                           (uint)mesh.triangles.Length / 3,
                                           has_normals, has_tangents, has_uvs, 
                                           mesh.vertices, mesh.triangles,
                                           mesh.normals, mesh.tangents, mesh.uv,
                                           (uint)mesh.subMeshCount);
    if (mesh_id == RoveSetup.ERROR) {
      Debug.LogError("Rove: Failed to create mesh; check 'Rove/Data/rove.log' " + 
                     "for details");
      return RoveSetup.ERROR;
    }
    uint current_triangle = 0;
    for (uint sub_mesh = 0; sub_mesh < mesh.subMeshCount; ++sub_mesh) {
      uint tri_count = (uint)mesh.GetTriangles((int)sub_mesh).Length / 3;
      RoveInternal.DefineSubMesh(mesh_id, sub_mesh, current_triangle, tri_count,
                                 m_material_indices[sub_mesh]);
      current_triangle += tri_count;
    }  
    return mesh_id;
  }
  ///
  public void FreeMesh()
  {
    if (m_mesh_id != RoveSetup.ERROR) {
      RoveInternal.FreeMesh(m_mesh_id);
    }
  }
  ///
  public void SetMeshTransform(float[] transform)
  {
    if (m_mesh_id != RoveSetup.ERROR) {
      RoveInternal.SetMeshTransform(m_mesh_id, transform);
    }
  }
  ///
  public void ChangeSubMeshMaterial(int sub_mesh_index, 
                                    RoveMaterial material)
  {
    uint material_index;
    if (material != null) {
      material_index = m_rove_materials.AddMaterial(m_materials[sub_mesh_index]);
      if (material_index == RoveSetup.ERROR) {
        m_rove_setup.m_setup_success = false;
        Destroy(m_rove_setup);
        return;
      }
    } else {
      material_index = 0;
    }
    RoveInternal.ChangeSubMeshMaterial(m_mesh_id, (uint)sub_mesh_index, 
                                       material_index);
  }
}
