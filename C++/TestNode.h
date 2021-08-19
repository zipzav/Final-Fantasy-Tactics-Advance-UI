#pragma once
#include <Godot.hpp>
#include <Node2D.hpp>

namespace godot
{
class TestNode : public Node2D
{
	private:
	GODOT_CLASS( TestNode, Node2D )

	public:
	static void _register_methods()
	{
		register_method( (char*)"_init", &TestNode::_init );
		register_method( (char*)"_process", &TestNode::_process );
		register_method( (char*)"_ready", &TestNode::_ready );
	}
	void _init() {};
	void _process( float delta ) {};
	void _ready() { Godot::print( "Hello World" ); };

	private:
	//bool loadSpritePaths();
	//bool parse_data( String filePath );
	//std::queue<UIConversationTextWidget> m_scene;
	//std::map<String, String> m_spriteMap;
	//Array m_spritePathArray;
};
}