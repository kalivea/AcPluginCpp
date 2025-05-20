#pragma once
#include "EnclosureType.h"

struct EnclosureParametersDTO
{
	EnclosureType enclosure_type;
	double ground_elevation;
	double structure_depth;
	double structure_width;
	double structure_height;
	double cushion_thickness;
	bool has_lattice_column;
	bool has_anti_uplift_pile;
	double anti_uplift_pile_length;
	double anti_uplift_pile_diam;
};

class POLADLL_ENCLOSURE_API EnclosureParameters
{
protected:
	EnclosureType enclosure_type_;		// 围护结构类型
	double ground_elevation_;			// 地面标高
	double structure_depth_;			// 主体结构埋深
	double structure_width_;			// 主体结构宽度
	double structure_height_;			// 主体结构深度
	double cushion_thickness_;			// 垫层厚度
	bool has_lattice_column_;			// 是否有格构柱
	bool has_anti_uplift_pile_;			// 是否有底部抗拔桩
	double anti_uplift_pile_length_;	// 抗拔桩长度
	double anti_uplift_pile_diam_;		// 抗拔桩直径

public:

	void SetEnclosureType(EnclosureType type);
	void SetGroundElevation(double ground_elevation);
	void SetStructureDepth(double structure_depth);
	void SetStructureWidth(double structure_width);
	void SetStructureHeight(double structure_height);
	void SetCushionThickness(double cushion_thickness);
	void SetHasLatticeColumn(bool has_lattice_column);
	void SetHasAntiUpliftPile(bool has_anti_uplift_pile);
	void SetAntiUpliftPileLength(double anti_uplift_pile_length);
	void SetAntiUpliftPileDiameter(double anti_uplift_pile_diam);

	EnclosureType GetEnclosureType() const;
	double GetGroundElevation() const;
	double GetStructureDepth() const;
	double GetStructureWidth() const;
	double GetStructureHeight() const;
	double GetCushionThickness() const;
	bool GetHasLatticeColumn() const;
	bool GetHasAntiUpliftPile() const;
	double GetAntiUpliftPileLength() const;
	double GetAntiUpliftPileDiameter() const;

	void FromDTO(const EnclosureParametersDTO& dto);
	EnclosureParametersDTO ToDTO() const;
};

