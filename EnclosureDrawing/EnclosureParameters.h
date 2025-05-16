#pragma once
#include "EnclosureType.h"

class POLADLL_ENCLOSURE_API EnclosureParameters
{
protected:
	EnclosureType enclosure_type_;		// Χ���ṹ����
	double ground_elevation_;			// ������
	double structure_depth_;			// ����ṹ����
	double structure_width_;			// ����ṹ���
	double structure_height_;			// ����ṹ���
	double cushion_thickness_;			// �����
	bool has_lattice_column_;			// �Ƿ��и���
	bool has_anti_uplift_pile_;			// �Ƿ��еײ�����׮
	double anti_uplift_pile_length_;	// ����׮����
	double anti_uplift_pile_diam_;		// ����׮ֱ��

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
};

