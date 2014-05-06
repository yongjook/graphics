#include "scene/mesh.hpp"


namespace _462 {

	bool Mesh::subdivide()
	{
		/*
	You should implement loop subdivision here.
	Triangles are stored in an std::vector<MeshTriangle> in 'triangles'.
	Vertices are stored in an std::vector<MeshVertex> in 'vertices'.
	Check mesh.hpp for the Mesh class definition.
	*/
	
		GenerateEdgeNeighbour();
		//create temp triangle. 
		MeshTriangle triangle_temp;

		//add odd vertices then even
		addOddVert();
		addEvenVert();
		
		//updating triangles and vertices after subdivision
		triangles=temp_triangles;
		vertices=temp_vertices;
		create_gl_data();
		return true;
	}

	void Mesh::GenerateEdgeNeighbour()
	{
		int num_vertices = vertices.size();
		int num_triangles = triangles.size();
		
		unsigned int tri_ind1[3]={0,1,2};
		unsigned int tri_ind2[3]={1,2,0};
		unsigned int tri_ind3[3]={2,0,1};
		
		std::vector< unsigned int > temp;

		MeshEdge edge_temp1,edge_temp2;

		MeshNeighbour neighbour_temp;
		for (int i=0;i<num_vertices;i++)
		{
			neighbour_temp.N=0;
			neighbours.push_back(neighbour_temp);
		}
		
		for (int i=0;i<num_vertices;i++)
		{
			for (int j=0;j<num_triangles;j++)
			{	
				int v1,v2;		
				bool isIn=IsVertexInTriangle(i,v1,v2,triangles[j]);
				if (isIn==true)
				{	
					AddNeighbour(i,v1,v2);			
				}	
			}
		}
		MeshFace temp_face;
		for (int i=0;i<num_triangles;i++)
		{
			for (int j=0;j<3;j++)
			{
				temp_face.vertices[j]=triangles[i].vertices[j];
			}
			faces.push_back(temp_face);

			for (int j=0;j<3;j++)
			{
				for (int k=0;k<num_triangles;k++)
				{
					faces[i].vertices_neighbour[tri_ind3[j]]=-1;
					int temp_in=IsEdgeInTriangle(faces[i].vertices[tri_ind1[j]],faces[i].vertices[tri_ind2[j]],triangles[k]);
					if (temp_in>=0 && k!=i)
					{
						faces[i].vertices_neighbour[tri_ind3[j]]=temp_in;
						break;
					}
				}
			}
		}
	}

	int Mesh::IsEdgeInTriangle(unsigned int v1,unsigned int v2,MeshTriangle t3)
	{
		
		unsigned int tri_ind2[3]={1,2,0};
		unsigned int tri_ind3[3]={2,0,1};
		for(int i=0;i<3;i++)
		{
			if (v1==t3.vertices[i])
			{
				if (v2==t3.vertices[tri_ind2[i]])
				return t3.vertices[tri_ind3[i]];
				if (v2==t3.vertices[tri_ind3[i]])
				return t3.vertices[tri_ind2[i]];
			}
		}
		return -1;
	}

	void Mesh::AddNeighbour(unsigned int v,int v1,int v2)
	{
		bool flag1 = false,flag2 = false;
		for (size_t i = 0; i < neighbours[v].indices.size(); i++)
		{
			flag1 = flag1 || (neighbours[v].indices[i] == v1);
			flag2 = flag2 || (neighbours[v].indices[i] == v2);
		}	
		if (!flag1) //if not found, then add it
		{
			neighbours[v].indices.push_back(v1);
		}
		if (!flag2) //if not found, then add it
		{
			neighbours[v].indices.push_back(v2);
		}
	}

	bool Mesh::IsVertexInTriangle(unsigned int v,int &v1,int &v2,MeshTriangle t3)
	{
		
		unsigned int tri_ind2[3]={1,2,0};
		unsigned int tri_ind3[3]={2,0,1};
		v1=-1;
		v2=-1;
		for(int i=0;i<3;i++)
		{
			if (v==t3.vertices[i])
			{
				v1=t3.vertices[tri_ind2[i]];
				v2=t3.vertices[tri_ind3[i]];
				return true;
			}
		}
		return false;

	}
	
	
	int Mesh::IsEdgeAdded(unsigned int v1,unsigned int v2,EdgeList e)
	{
		if (e.size()<1)
		return -1;
		
		for (int i=0;i<e.size();i++)
		{
			if (v1==e[i].vertices[0] && v2==e[i].vertices[1])
			return e[i].vertex_new;
			if (v2==e[i].vertices[0] && v1==e[i].vertices[1])
			return e[i].vertex_new;
		}
		return -1;
	}
	
	/*
	They are added on the first pass. We add a new vertex v for every edge ab, where a; b are vertices. We rst compute the position
for v, then split the edge to make 2 new edges, av and vb. The new vertex v is computed as a linear
combination of the vertices on the surrounding triangles.
	*/
	void Mesh::addOddVert()
	{
		int num_triangles=triangles.size();
		

		unsigned int tri_ind1[3]={0,1,2};
		unsigned int tri_ind2[3]={1,2,0};
		unsigned int tri_ind3[3]={2,0,1};
		
		GenerateEdgeNeighbour();
		MeshVertex vertex_temp1,vertex_temp2,vertex_temp3;
		MeshTriangle triangle_temp;
		//---------------------step 1: add odd vertices------------------------
		
		MeshVertex odd_vertices_temp;
		MeshEdge edge_temp;
		EdgeList edges;
		unsigned int new_vertex[3];
		temp_vertices=vertices;
		
		for (int i=0;i<num_triangles;i++)
		{
			for (int j=0;j<3;j++)
			{
				int new_ind=IsEdgeAdded(faces[i].vertices[tri_ind1[j]],faces[i].vertices[tri_ind2[j]],edges);
				new_vertex[tri_ind3[j]]=new_ind;
				
				if (new_ind<0)
				{
					if (faces[i].vertices_neighbour[tri_ind3[j]]>=0)
					{
						odd_vertices_temp.position=0.375*(vertices[faces[i].vertices[tri_ind1[j]]].position+vertices[faces[i].vertices[tri_ind2[j]]].position)+0.125*(vertices[faces[i].vertices_neighbour[tri_ind3[j]]].position+vertices[faces[i].vertices[tri_ind3[j]]].position);

						odd_vertices_temp.normal=0.375*(vertices[faces[i].vertices[tri_ind1[j]]].normal+vertices[faces[i].vertices[tri_ind2[j]]].normal)+0.125*(vertices[faces[i].vertices_neighbour[tri_ind3[j]]].normal+vertices[faces[i].vertices[tri_ind3[j]]].normal);
						odd_vertices_temp.tex_coord=0.375*(vertices[faces[i].vertices[tri_ind1[j]]].tex_coord+vertices[faces[i].vertices[tri_ind2[j]]].tex_coord)+0.125*(vertices[faces[i].vertices_neighbour[tri_ind3[j]]].tex_coord+vertices[faces[i].vertices[tri_ind3[j]]].tex_coord);
					}
					else
					{
						odd_vertices_temp.position=0.5*(vertices[faces[i].vertices[tri_ind1[j]]].position+vertices[faces[i].vertices[tri_ind2[j]]].position);
						odd_vertices_temp.normal=0.5*(vertices[faces[i].vertices[tri_ind1[j]]].normal+vertices[faces[i].vertices[tri_ind2[j]]].normal);
						odd_vertices_temp.tex_coord=0.5*(vertices[faces[i].vertices[tri_ind1[j]]].tex_coord+vertices[faces[i].vertices[tri_ind2[j]]].tex_coord);
					}
					
					new_vertex[tri_ind3[j]]=temp_vertices.size();
					temp_vertices.push_back(odd_vertices_temp);
					
					edge_temp.vertices[0]=faces[i].vertices[tri_ind1[j]];
					edge_temp.vertices[1]=faces[i].vertices[tri_ind2[j]];
					edge_temp.vertex_new=new_vertex[tri_ind3[j]];
					edges.push_back(edge_temp);
				}
			}
			triangle_temp.vertices[0]=faces[i].vertices[0];
			triangle_temp.vertices[1]=new_vertex[2];
			triangle_temp.vertices[2]=new_vertex[1];
			temp_triangles.push_back(triangle_temp);

			triangle_temp.vertices[0]=faces[i].vertices[1];
			triangle_temp.vertices[1]=new_vertex[0];
			triangle_temp.vertices[2]=new_vertex[2];
			temp_triangles.push_back(triangle_temp);

			triangle_temp.vertices[0]=faces[i].vertices[2];
			triangle_temp.vertices[1]=new_vertex[1];
			triangle_temp.vertices[2]=new_vertex[0];
			temp_triangles.push_back(triangle_temp);

			triangle_temp.vertices[0]=new_vertex[0];
			triangle_temp.vertices[1]=new_vertex[1];
			triangle_temp.vertices[2]=new_vertex[2];
			temp_triangles.push_back(triangle_temp);
		}
	}
	
	void Mesh::addEvenVert()
	{
		MeshVertex temp_evenVert;		//temp storage
		MeshVertexList even_vert;		//temp storage

		for (int i = 0;i < neighbours.size();i++)
		{
			int N=neighbours[i].indices.size();
			int ind_l=i;

			if (N==2)
			{
				temp_evenVert.position=0.75*(vertices[ind_l].position)+0.125*(vertices[neighbours[ind_l].indices[0]].position+vertices[neighbours[ind_l].indices[1]].position);
				temp_evenVert.normal=0.75*(vertices[ind_l].normal)+0.125*(vertices[neighbours[ind_l].indices[0]].normal+vertices[neighbours[ind_l].indices[1]].normal);
				temp_evenVert.tex_coord=0.75*(vertices[ind_l].tex_coord)+0.125*(vertices[neighbours[ind_l].indices[0]].tex_coord+vertices[neighbours[ind_l].indices[1]].tex_coord);
			}
			else 
			{
				double beta;
				double temp_beta=cos (2.0*3.1415926/N);
				beta=(5.0/8.0-(3.0/8.0+0.25*temp_beta)*(3.0/8.0+0.25*temp_beta))/N;
				temp_evenVert.position=(1-N*beta)*vertices[ind_l].position;
				temp_evenVert.normal=(1-N*beta)*vertices[ind_l].normal;
				temp_evenVert.tex_coord=(1-N*beta)*vertices[ind_l].tex_coord;
				
				for (int j = 0; j < N; j++ )
				{
					temp_evenVert.position+=beta*vertices[neighbours[ind_l].indices[j]].position;
					temp_evenVert.normal+=beta*vertices[neighbours[ind_l].indices[j]].normal;
					temp_evenVert.tex_coord+=beta*vertices[neighbours[ind_l].indices[j]].tex_coord;
				}
			}
			even_vert.push_back(temp_evenVert);
		}

		//---------------------combine and generate new mesh-----------------------
		for (int i=0;i<triangles.size();i++)
		{
			for (int j = 0;j<3;j++)
			{
				temp_vertices[triangles[i].vertices[j]]=even_vert[triangles[i].vertices[j]];
			}
		}
	}

} /* _462 */
