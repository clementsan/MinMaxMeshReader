#include <iostream>
#include <fstream>
#include <cstring>

#include <itkCovariantVector.h>

//Mesh Information
typedef itk::CovariantVector<double,3> CovariantVectorType;


void ReadMesh(std::string _FileName);
void ComputeMinMax();
void WriteAttributeFile(std::string _FileName);
void WritePointsFile(std::string _FileName);

int m_NbVertices;
std::vector<CovariantVectorType> m_vVertices;
CovariantVectorType LeftVertex, LeftSuperiorVertex, RightSuperiorVertex, PosteriorVertex, RightVertex, InferiorVertex, LeftAnteriorVertex, RightAnteriorVertex;
int LeftVertexIndex, LeftSuperiorVertexIndex, RightSuperiorVertexIndex, PosteriorVertexIndex, RightVertexIndex, InferiorVertexIndex, LeftAnteriorVertexIndex, RightAnteriorVertexIndex;
  
int main (int argc, char * argv[])
{
  if (argc == 4)
    {
      ReadMesh(argv[1]);
      std::cout<<"Nb vertices: "<<m_NbVertices<<std::endl;
      ComputeMinMax();
      WriteAttributeFile(argv[2]);
      WritePointsFile(argv[3]);
    }
  else
    std::cerr<<"MinMaxMeshReader <InputMesh.meta> <AttributeFile.txt> <PointsFile.lpts>"<<std::endl;

  return 0;
}

void ReadMesh(std::string _FileName)
{
  std::cout<<"Reading mesh..."<<std::endl;
  
  std::ifstream Infile;
  char Line[40];
  CovariantVectorType Vertex;
  int CurrentPoint;
   
  m_vVertices.clear();  
  
  Infile.open(_FileName.c_str());  
  //Skips the header and gets the number of points
  while ( strncmp (Line, "NPoints =", 9) && strncmp (Line, "NPoints=", 8))
    Infile.getline (Line, 40);    
  m_NbVertices = atoi(strrchr(Line,' '));
  
  Infile.getline ( Line, 40);

  //read the points in the file and set them as vertices
  for (int i = 0; i < m_NbVertices; i++ )
    {
      Infile >> CurrentPoint >> Vertex[0] >> Vertex[1] >> Vertex[2];
      m_vVertices.push_back(Vertex);
    }

  //close file
  Infile.close();
}

void ComputeMinMax()
{
  double Left = 1000.0, LeftSuperior = 1000.0, RightSuperior = 1000.0, Posterior = 1000.0;
  double Right = -1000.0, Inferior = -1000.0, LeftAnterior = -1000.0, RightAnterior = -1000.0;
  double XCenter;
  
  for (int i = 0; i < m_NbVertices; i++)
    {
      CovariantVectorType CurrentVertex = m_vVertices[i];
      if (CurrentVertex[0] < Left)
	{
	  LeftVertex = CurrentVertex;
	  Left = CurrentVertex[0];
	  LeftVertexIndex = i;
	}
      if (CurrentVertex[0] > Right)
	{
	  RightVertex = CurrentVertex;
	  Right = CurrentVertex[0];
	  RightVertexIndex = i;
	}
      if (CurrentVertex[2] < Posterior)
	{
	  PosteriorVertex = CurrentVertex;
	  Posterior = CurrentVertex[2];
	  PosteriorVertexIndex = i;
	}
      if (CurrentVertex[1] > Inferior)
	{
	  InferiorVertex = CurrentVertex;
	  Inferior = CurrentVertex[1];
	  InferiorVertexIndex = i;
	}      
    }
  XCenter = 0.5*(Left + Right);
  for (int i = 0; i < m_NbVertices; i++)
    {
      CovariantVectorType CurrentVertex = m_vVertices[i];
      if (CurrentVertex[0] < XCenter && CurrentVertex[1] < LeftSuperior)
	{
	  LeftSuperiorVertex = CurrentVertex;
	  LeftSuperior = CurrentVertex[1];
	  LeftSuperiorVertexIndex = i;
	}
      if (CurrentVertex[0] > XCenter && CurrentVertex[1] < RightSuperior)
	{
	  RightSuperiorVertex = CurrentVertex;
	  RightSuperior = CurrentVertex[1];
	  RightSuperiorVertexIndex = i;
	}
      if (CurrentVertex[0] < XCenter && CurrentVertex[2] > LeftAnterior)
	{
	  LeftAnteriorVertex = CurrentVertex;
	  LeftAnterior = CurrentVertex[2];
	  LeftAnteriorVertexIndex = i;
	}
      if (CurrentVertex[0] > XCenter && CurrentVertex[2] > RightAnterior)
	{
	  RightAnteriorVertex = CurrentVertex;
	  RightAnterior = CurrentVertex[2];
	  RightAnteriorVertexIndex = i;
	}
    }

  std::cout<<"LeftVertex: "<<LeftVertex<<std::endl;
  std::cout<<"RightVertex: "<<RightVertex<<std::endl;
  std::cout<<"InferiorVertex: "<<InferiorVertex<<std::endl;
  std::cout<<"PosteriorVertex: "<<PosteriorVertex<<std::endl;
  std::cout<<"LeftAnteriorVertex: "<<LeftAnteriorVertex<<std::endl;
  std::cout<<"RightAnteriorVertex: "<<RightAnteriorVertex<<std::endl;
  std::cout<<"LeftSuperiorVertex: "<<LeftSuperiorVertex<<std::endl;
  std::cout<<"RightSuperiorVertex: "<<RightSuperiorVertex<<std::endl;
}

void WriteAttributeFile(std::string _FileName)
{
  int i;
  std::ofstream outfile;
  
  outfile.open(_FileName.c_str());

  outfile<<"NUMBER_OF_POINTS="<<m_NbVertices<<std::endl<<"DIMENSION=1"<<std::endl<<"TYPE=Scalar"<<std::endl;
  for ( i = 0; i < m_NbVertices; i++)
    {
      if (i ==  LeftVertexIndex)
	outfile<<"1"<<std::endl;
      else if (i ==  RightVertexIndex)
	outfile<<"2"<<std::endl;
      else if (i ==  InferiorVertexIndex)
	outfile<<"3"<<std::endl;
      else if (i ==  PosteriorVertexIndex)
	outfile<<"4"<<std::endl;
      else if (i ==  LeftAnteriorVertexIndex)
	outfile<<"5"<<std::endl;
      else if (i ==  RightAnteriorVertexIndex)
	outfile<<"6"<<std::endl;
      else if (i ==  LeftSuperiorVertexIndex)
	outfile<<"7"<<std::endl;
      else if (i ==  RightSuperiorVertexIndex)
	outfile<<"8"<<std::endl;
      else
	outfile<<"0"<<std::endl;
    }
  outfile.close();
}

void WritePointsFile(std::string _FileName)
{
  std::ofstream outfile;
  
  outfile.open(_FileName.c_str());

  outfile<<LeftVertex[0]<<" "<<LeftVertex[1]<<" "<<LeftVertex[2]<<std::endl;
  outfile<<RightVertex[0]<<" "<<RightVertex[1]<<" "<<RightVertex[2]<<std::endl;
  outfile<<LeftAnteriorVertex[0]<<" "<<LeftAnteriorVertex[1]<<" "<<LeftAnteriorVertex[2]<<std::endl;
  outfile<<RightAnteriorVertex[0]<<" "<<RightAnteriorVertex[1]<<" "<<RightAnteriorVertex[2]<<std::endl;
  outfile<<LeftSuperiorVertex[0]<<" "<<LeftSuperiorVertex[1]<<" "<<LeftSuperiorVertex[2]<<std::endl;
  outfile<<RightSuperiorVertex[0]<<" "<<RightSuperiorVertex[1]<<" "<<RightSuperiorVertex[2]<<std::endl;
  //outfile<<PosteriorVertex[0]<<" "<<PosteriorVertex[1]<<" "<<PosteriorVertex[2]<<std::endl;
  //outfile<<InferiorVertex[0]<<" "<<InferiorVertex[1]<<" "<<InferiorVertex[2]<<std::endl;
  
  outfile.close();
}
