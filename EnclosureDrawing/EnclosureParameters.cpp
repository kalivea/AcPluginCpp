#include "stdafx.h"
#include "EnclosureParameters.h"

inline void EnclosureParameters::SetEnclosureType(EnclosureType type)
{
	enclosure_type_ = type;
}

inline void EnclosureParameters::SetGroundElevation(double ground_elevation)
{
	ground_elevation_ = ground_elevation;
}

inline void EnclosureParameters::SetStructureDepth(double structure_depth)
{
	structure_depth_ = structure_depth;
}

inline void EnclosureParameters::SetStructureWidth(double structure_width)
{
	structure_width_ = structure_width;
}

inline void EnclosureParameters::SetStructureHeight(double structure_height)
{
	structure_height_ = structure_height;
}

inline void EnclosureParameters::SetCushionThickness(double cushion_thickness)
{
	cushion_thickness_ = cushion_thickness;
}

inline void EnclosureParameters::SetHasLatticeColumn(bool has_lattice_column)
{
	has_lattice_column_ = has_lattice_column;
}

inline void EnclosureParameters::SetHasAntiUpliftPile(bool has_anti_uplift_pile)
{
	has_anti_uplift_pile_ = has_anti_uplift_pile;
}

inline void EnclosureParameters::SetAntiUpliftPileLength(double anti_uplift_pile_length)
{
	anti_uplift_pile_length_ = anti_uplift_pile_length;
}

inline void EnclosureParameters::SetAntiUpliftPileDiameter(double anti_uplift_pile_diam)
{
	anti_uplift_pile_diam_ = anti_uplift_pile_diam;
}


inline EnclosureType EnclosureParameters::GetEnclosureType() const
{
	return enclosure_type_;
}

inline double EnclosureParameters::GetGroundElevation() const
{
	return ground_elevation_;
}

inline double EnclosureParameters::GetStructureDepth() const
{
	return structure_depth_;
}

inline double EnclosureParameters::GetStructureWidth() const
{
	return structure_width_;
}

inline double EnclosureParameters::GetStructureHeight() const
{
	return structure_height_;
}

inline double EnclosureParameters::GetCushionThickness() const
{
	return cushion_thickness_;
}

inline bool EnclosureParameters::GetHasLatticeColumn() const
{
	return has_lattice_column_;
}

inline bool EnclosureParameters::GetHasAntiUpliftPile() const
{
	return has_anti_uplift_pile_;
}

inline double EnclosureParameters::GetAntiUpliftPileLength() const
{
	return anti_uplift_pile_length_;
}

inline double EnclosureParameters::GetAntiUpliftPileDiameter() const
{
	return anti_uplift_pile_diam_;
}
