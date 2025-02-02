/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkPolyDataMapper.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkPolyDataMapper
 * @brief   map vtkPolyData to graphics primitives
 *
 * vtkPolyDataMapper is a class that maps polygonal data (i.e., vtkPolyData)
 * to graphics primitives. vtkPolyDataMapper serves as a superclass for
 * device-specific poly data mappers, that actually do the mapping to the
 * rendering/graphics hardware/software.
 */

#ifndef vtkPolyDataMapper_h
#define vtkPolyDataMapper_h

#include "vtkMapper.h"
#include "vtkRenderingCoreModule.h" // For export macro
//#include "vtkTexture.h" // used to include texture unit enum.

VTK_ABI_NAMESPACE_BEGIN
class vtkPolyData;
class vtkRenderer;
class vtkRenderWindow;

class VTKRENDERINGCORE_EXPORT vtkPolyDataMapper : public vtkMapper
{
public:
  static vtkPolyDataMapper* New();
  vtkTypeMacro(vtkPolyDataMapper, vtkMapper);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  /**
   * Implemented by sub classes. Actual rendering is done here.
   */
  virtual void RenderPiece(vtkRenderer*, vtkActor*){};

  /**
   * This calls RenderPiece (in a for loop if streaming is necessary).
   */
  void Render(vtkRenderer* ren, vtkActor* act) override;

  ///@{
  /**
   * Specify the input data to map.
   */
  void SetInputData(vtkPolyData* in);
  vtkPolyData* GetInput();
  ///@}

  ///@{
  /**
   * Bring this algorithm's outputs up-to-date.
   */
  void Update(int port) override;
  void Update() override;
  vtkTypeBool Update(int port, vtkInformationVector* requests) override;
  vtkTypeBool Update(vtkInformation* requests) override;
  ///@}

  ///@{
  /**
   * If you want only a part of the data, specify by setting the piece.
   */
  vtkSetMacro(Piece, int);
  vtkGetMacro(Piece, int);
  vtkSetMacro(NumberOfPieces, int);
  vtkGetMacro(NumberOfPieces, int);
  vtkSetMacro(NumberOfSubPieces, int);
  vtkGetMacro(NumberOfSubPieces, int);
  ///@}

  ///@{
  /**
   * Set the number of ghost cells to return.
   */
  vtkSetMacro(GhostLevel, int);
  vtkGetMacro(GhostLevel, int);
  ///@}

  ///@{
  /**
   * Accessors / Mutators for handling seams on wrapping surfaces. Letters U and V stand for
   * texture coordinates (u,v).
   *
   * @note Implementation taken from the work of Marco Tarini:
   * Cylindrical and Toroidal Parameterizations Without Vertex Seams
   * Journal of Graphics Tools, 2012, number 3, volume 16, pages 144-150.
   */
  vtkSetMacro(SeamlessU, bool);
  vtkGetMacro(SeamlessU, bool);
  vtkBooleanMacro(SeamlessU, bool);
  vtkSetMacro(SeamlessV, bool);
  vtkGetMacro(SeamlessV, bool);
  vtkBooleanMacro(SeamlessV, bool);
  ///@}

  /**
   * Return bounding box (array of six doubles) of data expressed as
   * (xmin,xmax, ymin,ymax, zmin,zmax).
   */
  double* GetBounds() VTK_SIZEHINT(6) override;
  void GetBounds(double bounds[6]) override { this->Superclass::GetBounds(bounds); }

  /**
   * Make a shallow copy of this mapper.
   */
  void ShallowCopy(vtkAbstractMapper* m) override;

  /**
   * Select a data array from the point/cell data
   * and map it to a generic vertex attribute.
   * vertexAttributeName is the name of the vertex attribute.
   * dataArrayName is the name of the data array.
   * fieldAssociation indicates when the data array is a point data array or
   * cell data array (vtkDataObject::FIELD_ASSOCIATION_POINTS or
   * (vtkDataObject::FIELD_ASSOCIATION_CELLS).
   * componentno indicates which component from the data array must be passed as
   * the attribute. If -1, then all components are passed.
   * Currently only point data is supported.
   */
  virtual void MapDataArrayToVertexAttribute(const char* vertexAttributeName,
    const char* dataArrayName, int fieldAssociation, int componentno = -1);

  // Specify a data array to use as the texture coordinate
  // for a named texture. See vtkProperty.h for how to
  // name textures.
  virtual void MapDataArrayToMultiTextureAttribute(
    const char* textureName, const char* dataArrayName, int fieldAssociation, int componentno = -1);

  /**
   * Remove a vertex attribute mapping.
   */
  virtual void RemoveVertexAttributeMapping(const char* vertexAttributeName);

  /**
   * Remove all vertex attributes.
   */
  virtual void RemoveAllVertexAttributeMappings();

  /**
   * see vtkAlgorithm for details
   */
  vtkTypeBool ProcessRequest(
    vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;

protected:
  vtkPolyDataMapper();
  ~vtkPolyDataMapper() override = default;

  /**
   * Called in GetBounds(). When this method is called, the consider the input
   * to be updated depending on whether this->Static is set or not. This method
   * simply obtains the bounds from the data-object and returns it.
   */
  virtual void ComputeBounds();

  int Piece;
  int NumberOfPieces;
  int NumberOfSubPieces;
  int GhostLevel;
  bool SeamlessU, SeamlessV;

  int FillInputPortInformation(int, vtkInformation*) override;

private:
  vtkPolyDataMapper(const vtkPolyDataMapper&) = delete;
  void operator=(const vtkPolyDataMapper&) = delete;
};

VTK_ABI_NAMESPACE_END
#endif
