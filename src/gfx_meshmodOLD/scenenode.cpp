#include "meshmod.h"
#include "scenenode.h"
#include <stack>

namespace MeshMod {

void SceneNode::visitDescendents(Math::mat4x4 const& rootMatrix, std::function<void(SceneNode const&, Math::mat4x4 const&)> func ) const
{
	std::stack<MeshMod::SceneNode const*> nodeStack;
	std::stack<Math::mat4x4> matrixStack;

	nodeStack.push(this);
	matrixStack.push(rootMatrix);

	while (!nodeStack.empty())
	{
		auto const node = nodeStack.top();
		nodeStack.pop();
		auto const parentMatrix = matrixStack.top();
		matrixStack.pop();

		Math::mat4x4 local = node->transform.MakeMatrix();
		Math::mat4x4 world = parentMatrix * local;

		func(*node, world);

		for (auto i = 0u; i < node->getChildCount(); ++i)
		{
			nodeStack.push(node->getChild(i).get());
			matrixStack.push(world);
		}
	}
}

void SceneNode::mutateDescendents(Math::mat4x4 const& rootMatrix, std::function<void(SceneNode&, Math::mat4x4 const&)> func)
{
	std::stack<MeshMod::SceneNode*> nodeStack;
	std::stack<Math::mat4x4> matrixStack;

	nodeStack.push(this);
	matrixStack.push(rootMatrix);

	while (!nodeStack.empty())
	{
		auto const node = nodeStack.top();
		nodeStack.pop();
		auto const parentMatrix = matrixStack.top();
		matrixStack.pop();

		Math::mat4x4 local = node->transform.MakeMatrix();
		Math::mat4x4 world = local * parentMatrix;

		func(*node, world);

		for (auto i = 0u; i < node->getChildCount(); ++i)
		{
			nodeStack.push(node->getChild(i).get());
			matrixStack.push(world);
		}
	}
}


void SceneNode::visitObjects( std::function<void(SceneObject::ConstPtr)> func ) const
{
	for(auto obj : objects)
	{
		func(obj);
	}
}
void SceneNode::mutateObjects(std::function<void(SceneObject::Ptr)> func)
{
	for (auto obj : objects)
	{
		func(obj);
	}
}

} // end namespace