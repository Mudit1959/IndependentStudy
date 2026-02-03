#include "Entity.h"

Entity::Entity(std::shared_ptr<Mesh> inMesh, std::shared_ptr<Material> inMaterial)
{
	mesh = inMesh;
	material = inMaterial;
}

void Entity::Draw() 
{
	Graphics::Context->IASetInputLayout(material->GetInputLayout().Get());
	Graphics::Context->VSSetShader(material->GetVertexShader().Get(), 0, 0);
	Graphics::Context->PSSetShader(material->GetPixelShader().Get(), 0, 0);

	mesh->Draw();
}

Transform* Entity::GetTransform() { return &transform; }