#pragma once

#include "NodeSelector.h"

#include <Godot.hpp>
#include <Sprite.hpp>
#include <String.hpp>
#include <Label.hpp>

#include <array>
#include <functional>
#include <queue>
namespace godot
{
class UIConversationTextWidget : public Node2D
{
	private:
		GODOT_CLASS( UIConversationTextWidget, Node2D )

	public:
		UIConversationTextWidget();

		void update_text( String text );
		void show_all_text();
		void hide_all_text();
		Label* get_text_node() { return Object::cast_to<Label>( get_node( "Text" ) ); };
		void showBouncingButton( bool isShow );
		static void _register_methods();		
		void _init();
		void _ready();
		void _process( float delta );

	private:
		bool m_ScrollingText{ false };
		bool m_animateBouncingButton;
		std::array<float, 2> m_bouncingButtonPosition{ 0.0f, -5.0f };
		int m_bouncingAnimationCounter = 0;
		float m_bouncingAnimationTimer = 0.0f;
};
}
