#include "UIConversationTextWidget.h"

#include <NodePath.hpp>
#include <string>
using namespace godot;

//-------------------------------------------------------------------
void UIConversationTextWidget::_register_methods()
{
	register_method( (char*)"_ready", &UIConversationTextWidget::_ready );
	register_method( (char*)"_init", &UIConversationTextWidget::_init );
	register_method( (char*)"_process", &UIConversationTextWidget::_process );

};

//-------------------------------------------------------------------
UIConversationTextWidget::UIConversationTextWidget(){};

//-------------------------------------------------------------------
void UIConversationTextWidget::_init(){}

//-------------------------------------------------------------------
void UIConversationTextWidget::_ready()
{
	float original_position = Object::cast_to<Sprite>( get_node( "DownwardTriangle" ) )->get_position().y;
	m_bouncingButtonPosition[0] = original_position;
	m_bouncingButtonPosition[1] = original_position - 5.0f;
}

//-------------------------------------------------------------------
void UIConversationTextWidget::_process( float delta )
{
	if( m_bouncingAnimationTimer >= 0.4f )
	{
		m_bouncingAnimationTimer = 0.0f;
		auto node = Object::cast_to<Sprite>( get_node( "DownwardTriangle" ) );
		node->set_position( Vector2( node->get_position().x, m_bouncingButtonPosition[m_bouncingAnimationCounter] ) );
		m_bouncingAnimationCounter = ( m_bouncingAnimationCounter + 1 ) % 2;
	}
	else
		m_bouncingAnimationTimer += delta;
}

//-------------------------------------------------------------------
void UIConversationTextWidget::update_text( String text )
{
	//Ideally should have been a member variable set at _ready()
	//but there is a bug that causes the pointer to become invalid
	auto textbox = Object::cast_to<Label>( get_node( "Text" ) );
	textbox->set_text( text );
}

//-------------------------------------------------------------------
void UIConversationTextWidget::show_all_text()
{
	//Ideally should have been a member variable set at _ready()
	//but there is a bug that causes the pointer to become invalid
	auto text = Object::cast_to<Label>( get_node( "Text" ) );
	text->set_percent_visible( 1.0f );
}

//-------------------------------------------------------------------
void UIConversationTextWidget::hide_all_text()
{
	//Ideally should have been a member variable set at _ready()
	//but there is a bug that causes the pointer to become invalid
	auto text = Object::cast_to<Label>( get_node( "Text" ) );
	text->set_percent_visible( 0.0f );
}

//-------------------------------------------------------------------
void UIConversationTextWidget::showBouncingButton( bool isShow )
{
	auto node = Object::cast_to<Sprite>( get_node( "DownwardTriangle" ) );
	node->set_visible( isShow );
	m_animateBouncingButton = isShow;
}
