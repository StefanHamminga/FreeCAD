/***************************************************************************
 *   Copyright (c) 2013 Luke Parry <l.parry@warwick.ac.uk>                 *
 *                                                                         *
 *   This file is part of the FreeCAD CAx development system.              *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License as published by the Free Software Foundation; either          *
 *   version 2 of the License, or (at your option) any later version.      *
 *                                                                         *
 *   This library  is distributed in the hope that it will be useful,      *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Library General Public License for more details.                  *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this library; see the file COPYING.LIB. If not,    *
 *   write to the Free Software Foundation, Inc., 59 Temple Place,         *
 *   Suite 330, Boston, MA  02111-1307, USA                                *
 *                                                                         *
 ***************************************************************************/

#ifndef _TECHDRAW_FEATUREVIEWGROUP_H_
#define _TECHDRAW_FEATUREVIEWGROUP_H_

#include <string>
# include <QRectF>
#include <App/DocumentObject.h>
#include <App/PropertyStandard.h>

#include <Base/BoundBox.h>
#include <Base/Matrix.h>
#include <Base/Vector3D.h>

//#include "Cube.h"
#include "DrawViewCollection.h"

namespace TechDraw
{

class DrawProjGroupItem;
class Cube;

/**
 * Class super-container for managing a collection of DrawProjGroupItem
 * Page Features
 */
class TechDrawExport DrawProjGroup : public TechDraw::DrawViewCollection
{
    PROPERTY_HEADER_WITH_OVERRIDE(TechDraw::DrawProjGroup);

public:
    /// Constructor
    DrawProjGroup();
    ~DrawProjGroup();

    App::PropertyEnumeration ProjectionType;

    App::PropertyBool AutoDistribute;
    /// Default horizontal spacing between adjacent views on Drawing, in mm
    App::PropertyFloat spacingX;
    /// Default vertical spacing between adjacent views on Drawing, in mm
    App::PropertyFloat spacingY;

    App::PropertyLink Anchor; /// Anchor Element to align views to

    Base::BoundBox3d getBoundingBox() const;
    double calculateAutomaticScale() const;
    virtual QRectF getRect(void) const override;
    virtual bool checkFit(TechDraw::DrawPage* p) const override;
    /// Check if container has a view of a specific type
    bool hasProjection(const char *viewProjType) const;

    App::DocumentObject * getProjObj(const char *viewProjType) const;
    DrawProjGroupItem* getProjItem(const char *viewProjType) const;

    //! Adds a projection to the group
    /*!
     * \return pointer to the new view
     */
    App::DocumentObject * addProjection(const char *viewProjType);

    //! Removes a projection from the group
    /*!
     * \return number of projections remaining
     */
    int removeProjection(const char *viewProjType);

    int purgeProjections();
    /// Automatically position child views
    bool distributeProjections(void);
    void resetPositions(void);

    short mustExecute() const override;
    /** @name methods overide Feature */
    //@{
    /// recalculate the Feature
    virtual void onDocumentRestored() override;
    virtual App::DocumentObjectExecReturn *execute(void) override;
    //@}

    /// returns the type name of the ViewProvider
    virtual const char* getViewProviderName(void) const override {
        return "TechDrawGui::ViewProviderProjGroup";
    }
    //return PyObject as DrawProjGroupPy
    virtual PyObject *getPyObject(void) override;

    /// Determines either "First Angle" or "Third Angle".
    App::Enumeration usedProjectionType(void);

    /// Allowed projection types - either Document, First Angle or Third Angle
    static const char* ProjectionTypeEnums[];

    bool hasAnchor(void);
    void setAnchorDirection(Base::Vector3d dir);
    Base::Vector3d getAnchorDirection(void);
    TechDraw::DrawProjGroupItem* getAnchor(void);

    void updateSecondaryDirs();

    void rotateRight(void);
    void rotateLeft(void);
    void rotateUp(void);
    void rotateDown(void);
    void spinCW(void);
    void spinCCW(void);
    
    void setTable(Base::Vector3d dir, Base::Vector3d rot);
    void setConfig(std::string cfg);
    void resetTable(void);
    void dumpISO(char * title);

protected:
    void onChanged(const App::Property* prop) override;

    //! Moves anchor view to keep our bounding box centre on the origin
    void moveToCentre();

    /// Annoying helper - keep in sync with DrawProjGroupItem::TypeEnums
    /*!
     * \todo {See note regarding App::PropertyEnumeration on my wiki page http://freecadweb.org/wiki/User:Ian.rees}
     * \return true iff 'in' is a valid name for an orthographic/isometric view
     */
    bool checkViewProjType(const char *in);

    void arrangeViewPointers(DrawProjGroupItem *viewPtrs[10]) const;

    /// Populates array of 10 BoundBox3d's given DrawProjGroupItem *s
    /*!
     * If documentScale is set, then returned bounding boxes are scaled as in
     * the Drawing.  Otherwise, the dimensions are as in object space.
     */
    void makeViewBbs(DrawProjGroupItem *viewPtrs[10],
                     Base::BoundBox3d bboxes[10],
                     bool documentScale = true) const;

    /// Helper for calculateAutomaticScale
    /*!
     * Returns a width and height in object-space scale, for the enabled views
     * without accounting for their actual X and Y positions or borders.
     */
    void minimumBbViews(DrawProjGroupItem *viewPtrs[10],
                       double &width, double &height) const;

    /// Returns pointer to our page, or NULL if it couldn't be located
    TechDraw::DrawPage * getPage(void) const;
    void updateChildren(double scale);
    
    //TechDraw::Cube m_cube;
    TechDraw::Cube* m_cube;
    static const std::map<std::string,Base::Vector3d> m_frameToStdDir;    //for initializing cube and
    static const std::map<std::string,Base::Vector3d> m_frameToStdRot;    //creating DPGI's
    static const std::map<std::string,std::string> m_dirRotToConfig;
    static const std::map<std::string,std::string> m_frontTopToConfig;
};

} //namespace TechDraw

#endif // _TECHDRAW_FEATUREVIEWGROUP_H_
