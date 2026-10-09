#include "pack.h"
packetClass::packetClass()
{
	code=width=height=length=weight=topPlacementPriority=orderId=0;
	manualExportOrder=0;
	YCG=0;
}
packetClass::packetClass(int code_,QString profile_,int width_,int height_,int length_,int weight_,int topPlacementPriority_,int orderId_)
{
	code									=code_;
	profile								=profile_;
	width									=width_;
	height								=height_;
	length								=length_;
	weight								=weight_;
	topPlacementPriority	=topPlacementPriority_;
	orderId								=orderId_;
	YCG										=woodWidth+0.5*height;
	manualExportOrder=0;
}
