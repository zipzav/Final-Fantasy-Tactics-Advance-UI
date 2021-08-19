#include "NodeSelector.h"

#include <Godot.hpp>

#include <Object.hpp>
#include <Sprite.hpp>

#include <iostream>
using namespace godot;

//-------------------------------------------------------------------
void NodeSelector::_register_methods()
{
	//register_method( (char*)"_process", &UIConversationTextWidget::_process );
	//register_method( (char*)"_init", &UIConversationTextWidget::_init );
	register_method( (char*)"SetSelector", &NodeSelector::SetSelector );
};

//-------------------------------------------------------------------
bool NodeSelector::SetSelector( String childName )
{
	bool nodeFound = false;
	for( int i = 0; i < get_child_count(); i++ )
	{
		auto child = Object::cast_to<CanvasItem>( get_child( i ) );

		if( child->get_name() == childName )
		{
			child->set_visible( true );
			m_selectedChild = Object::cast_to<Node>( child );
			Godot::print( "Found Node: " + childName );
		}
		else
		{
			child->set_visible( false );
		}
	}

	return nodeFound;
};

