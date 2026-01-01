#include "ComponentFactory.h"

ComponentFactory::ComponentFactory(Registry* registry)
{
	reg = registry;
}

Entity ComponentFactory::createObject(MaterialAsset* materialAsset, MeshAsset* meshAsset, SpatialData initialPos)
{

	// Creates entity
	Entity ent = reg->createEntity();


	// Transform component
	reg->addComponent(ent, initialPos);


	// Mesh component
	CMeshNonIndexed mesh;
	mesh.mesh = meshAsset;
	mesh.material = materialAsset;

	reg->addComponent(ent, mesh);

	



	return ent;
}
