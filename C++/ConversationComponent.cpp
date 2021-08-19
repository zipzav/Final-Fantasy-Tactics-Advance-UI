#include "ConversationComponent.h"

#include <Dictionary.hpp>
#include <File.hpp>
#include <JSON.hpp>
#include <JSONParseResult.hpp>
#include <Input.hpp>
#include <InputEvent.hpp>
#include <InputEventKey.hpp>
#include <Texture.hpp>
#include <ResourceLoader.hpp>

#include<iostream>
#include<string>
using namespace godot;

namespace
{

} //namespace

//-------------------------------------------------------------------
void ConversationComponent::_register_methods()
{
	register_method( (char*)"_ready", &ConversationComponent::_ready );
	register_method( (char*)"_init", &ConversationComponent::_init );
	register_method( (char*)"_process", &ConversationComponent::_process );
	register_method( (char*)"debug_button_press", &ConversationComponent::debug_button_press );
	register_method( (char*)"on_shown_all_text", &ConversationComponent::on_shown_all_text );
	register_method( (char*)"show_all_text", &ConversationComponent::show_all_text );
	register_method( (char*)"update_UIString", &ConversationComponent::update_UIString );
	register_method( (char*)"on_done_intro_animation", &ConversationComponent::on_done_intro_animation );
}

//-------------------------------------------------------------------
void ConversationComponent::_init()
{
	std::cout << "_init: void ConversationComponent" << std::endl;
}

//-------------------------------------------------------------------
void ConversationComponent::_ready()
{
	std::cout << "_ready" << std::endl;
	load_sprite_paths();

	m_originSelector = static_cast<NodeSelector*>( get_node( s_originSelectorPath ) );
	m_debugButton = static_cast<Button*>( get_node( "DebugButton" ) );
	m_debugButton->connect( "pressed", this, "debug_button_press" );

	//debugCounter = 0;
	//m_originSelector->SetSelector( originArray[debugCounter] );

	m_UIAnimationTween = Tween::_new();
	add_child( m_UIAnimationTween );
	add_user_signal( "continue_button_pressed" );

	m_audioPlayer = static_cast<AudioStreamPlayer*>( get_node( s_audioPlayerPath ) );
	auto resourceLoader = ResourceLoader::get_singleton();
	m_scrollPlayer = resourceLoader->load( s_textScrollSoundPath );
	m_audioPlayer->set_stream( m_scrollPlayer );
}

//-------------------------------------------------------------------
void ConversationComponent::_process( float delta )
{
	Input* input = Input::get_singleton();
	if( input->is_action_just_released( "continue" ) )
	{
		emit_signal( "continue_button_pressed" );
	}
}

//-------------------------------------------------------------------
void ConversationComponent::_input( InputEvent event )
{
	if( event.is_action_released( "continue" )  && !event.is_echo() )
	{
		emit_signal( "continue_button_pressed" );
	}
}

//-------------------------------------------------------------------
void ConversationComponent::debug_button_press()
{
	//debugCounter = (debugCounter + 1) % m_originSelector->get_child_count();
	//m_originSelector->SetSelector( originArray[debugCounter] );
	
	m_conversationStep = 0;
	m_speechIncrement = 0;

	if( !parse_data( s_testConversationPath ) )
		Godot::print( "Error parsing conversation file: " + s_testConversationPath );
	conversation_step();
	update_UIString();
}

//-------------------------------------------------------------------
bool ConversationComponent::load_sprite_paths()
{
	File* file = File::_new();
	if( Error::OK != file->open( String( s_spritePathsPath ), File::ModeFlags::READ ) )
	{
		Godot::print( "File not found: " + s_spritePathsPath );
		return false;
	}

	m_spritePathArray = JSON::get_singleton()->parse( file->get_as_text() )->get_result();
	file->close();
	return true;
}

//-------------------------------------------------------------------
bool ConversationComponent::parse_data( String filePath )
{
	File* file = File::_new();
	if( Error::OK != file->open( filePath, File::ModeFlags::READ ) )
	{
		Godot::print( "File not found: " + filePath );
		return false;
	}
	Dictionary conversationdict = JSON::get_singleton()->parse( file->get_as_text() )->get_result();

	//Create m_spriteMap
	auto characterArray = conversationdict["characterArray"].operator godot::Array();
	auto resourceLoader = ResourceLoader::get_singleton();
	for( int i = 0; i < characterArray.size(); i++ )
	{
		Dictionary spritedict = m_spritePathArray[characterArray[i].operator String ()];
		String spritepath = spritedict["filepath"].operator String();

		m_spriteMap.insert( std::pair<String, Ref<Texture>>( characterArray[i], resourceLoader->load( spritepath ) ) );
	}

	//Create m_scene
	m_conversationArray = conversationdict["conversation"].operator godot::Array();
	file->close();
	return true;
}

//-------------------------------------------------------------------
void ConversationComponent::conversation_step()
{
	m_currentDialogue = m_conversationArray[m_conversationStep];
	m_character = m_currentDialogue["character"];
	m_UIOrigin = m_currentDialogue["origin"];
	m_currentSpeech = m_currentDialogue["speech"];

	m_originSelector->SetSelector( m_UIOrigin );
	auto selectedOriginNode = m_originSelector->get_selected_child();

	m_sizeSelector = static_cast<NodeSelector*>( selectedOriginNode->get_node( NodePath( "BGSelector") ) );
	
	m_characterSprite = static_cast<Sprite*>( selectedOriginNode->get_node( NodePath( "CharacterPortrait" ) ) );
	m_characterSprite->set_texture( m_spriteMap[m_character] );
	m_characterSprite->set_visible( true );
}

//-------------------------------------------------------------------
void ConversationComponent::update_UIString()
{
	disconnect( "continue_button_pressed", this, "update_UIString" );



	if( m_speechIncrement == m_currentSpeech.size() )
	{
		m_conversationStep++;
		if( m_conversationArray.size() == m_conversationStep )
		{
			on_conversation_end();
			return;
		}
		m_speechIncrement = 0;
		conversation_step();
	}

	m_currentSpeechString = m_currentSpeech[m_speechIncrement];

	m_sizeSelector->SetSelector( String( "onesize" ) );
	m_UITextWidget = static_cast<UIConversationTextWidget*>( m_sizeSelector->get_selected_child() );
	m_UITextWidget->update_text( m_currentSpeechString );

	auto labelNode = m_UITextWidget->get_text_node();
	if( labelNode->get_line_count() != labelNode->get_visible_line_count() )
	{
		m_sizeSelector->SetSelector( String( "twosize" ) );
		m_UITextWidget = static_cast<UIConversationTextWidget*>( m_sizeSelector->get_selected_child() );
		m_UITextWidget->update_text( m_currentSpeechString );
		labelNode = m_UITextWidget->get_text_node();
	}
	if( labelNode->get_line_count() != labelNode->get_visible_line_count() )
	{
		m_sizeSelector->SetSelector( String( "threesize" ) );
		m_UITextWidget = static_cast<UIConversationTextWidget*>( m_sizeSelector->get_selected_child() );
		m_UITextWidget->update_text( m_currentSpeechString );
	}

	m_UITextWidget->hide_all_text();
	m_UITextWidget->showBouncingButton( false );
	if( m_speechIncrement == 0 )
		start_intro_animation();
	else
		start_text_scrolling();
}

//-------------------------------------------------------------------
void ConversationComponent::start_text_scrolling()
{
	auto length = m_currentSpeechString.length();
	m_UIAnimationTween->interpolate_property( m_UITextWidget->get_text_node(), "percent_visible", 0.0f, 1.0f, static_cast<float>( length ) / m_letterPerSecond );
	m_UIAnimationTween->start();
	m_UIAnimationTween->connect( "tween_completed", this, "on_shown_all_text" );
	m_speechIncrement++;

	connect( "continue_button_pressed", this, "show_all_text" );
	m_audioPlayer->play();
}

//-------------------------------------------------------------------
void ConversationComponent::on_shown_all_text()
{
	m_audioPlayer->stop();
	disconnect( "continue_button_pressed", this, "show_all_text" );

	m_UITextWidget->showBouncingButton( true );
	m_UIAnimationTween->disconnect( "tween_completed", this, "on_shown_all_text" );
	
	connect( "continue_button_pressed", this, "update_UIString" );
}

//-------------------------------------------------------------------
void ConversationComponent::show_all_text()
{
	disconnect( "continue_button_pressed", this, "show_all_text" );

	m_UIAnimationTween->disconnect( "tween_completed", this, "on_shown_all_text" );
	m_UIAnimationTween->remove_all();
	m_UITextWidget->show_all_text();
	m_UITextWidget->showBouncingButton( true );
	m_audioPlayer->stop();

	connect( "continue_button_pressed", this, "update_UIString" );
}

//-------------------------------------------------------------------
void ConversationComponent::on_conversation_end()
{
	m_UITextWidget->set_visible( false );
	m_characterSprite->set_visible( false );
	m_UITextWidget->showBouncingButton( false );

	for( auto& element : m_spriteMap )
	{
		element.second->unreference();
	}
	m_spriteMap.erase( m_spriteMap.begin(), m_spriteMap.end() );
}

//-------------------------------------------------------------------
void ConversationComponent::start_intro_animation()
{
	m_UIAnimationTween->interpolate_property( m_UITextWidget, "scale:x", 2.0f, 1.0f, 0.10f );
	m_UIAnimationTween->start();
	m_UIAnimationTween->connect( "tween_completed", this, "on_done_intro_animation" );
}

//-------------------------------------------------------------------
void ConversationComponent::on_done_intro_animation()
{
	m_UIAnimationTween->disconnect( "tween_completed", this, "on_done_intro_animation" );
	start_text_scrolling();
}