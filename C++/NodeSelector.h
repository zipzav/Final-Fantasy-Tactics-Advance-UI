#pragma once

#include <Godot.hpp>

#include <Array.hpp>
#include <Node2D.hpp>
#include <String.hpp>
namespace godot
{

class NodeSelector : public Node2D
{
	GODOT_CLASS( NodeSelector, Node2D )

	public:
		static void _register_methods();
		bool SetSelector( String childName );
		Node* get_selected_child() { return m_selectedChild; };

	private:
	Node* m_selectedChild;
};
}