#pragma once
#include "NodeSelector.h"
#include "UIConversationTextWidget.h"
#include <Godot.hpp>

#include <AudioStreamPlayer.hpp>
#include <AudioStream.hpp>
#include <Button.hpp>
#include <Node2D.hpp>
#include <Texture.hpp>
#include <Tween.hpp>
#include <String.hpp>

#include <array>
#include <map>
#include <queue>

namespace godot
{
class ConversationComponent : public Node2D
{
	const std::array<String, 2> originArray = { "TopRight", "BottomRight" };
	private:
		GODOT_CLASS( ConversationComponent, Node2D )

	public:
		static void _register_methods();
		void _ready();
		void _init();
		void _process( float delta );
		void _input( InputEvent event );

		void conversation_step();
	public:
		float	m_letterPerSecond;
	private:
		const String s_convesationFilePath{ "test.json" };
		const String s_spritePathsPath{ "GUI/CharacterSprites/sprites.json" };
		const NodePath s_originSelectorPath{ "OriginSelector" };
		const String s_testConversationPath{ "Conversation/conversation1.json" };
		const NodePath s_audioPlayerPath{ "AudioPlayer" };
		const String s_textScrollSoundPath{ "Sound/text_scroll.wav" };

	private:
		void debug_button_press();
		bool load_sprite_paths();
		bool parse_data( String filePath );
		void update_UIString();

		void on_shown_all_text();
		void show_all_text();

		void on_conversation_end();

		void start_text_scrolling();

		void start_intro_animation();
		void on_done_intro_animation();

	private:
		bool m_hasSeenFullText;
		int debugCounter;
		int m_conversationStep;
		int m_speechIncrement;

		Dictionary m_spritePathArray; 
		std::map<String, Ref<Texture>> m_spriteMap;
		Array m_conversationArray;

		Button* m_debugButton;
		NodeSelector* m_originSelector;
		NodeSelector* m_sizeSelector;
		UIConversationTextWidget* m_UITextWidget;
		Sprite* m_characterSprite;

		Array m_currentSpeech;
		Dictionary m_currentDialogue;
		String m_character;
		String m_UIOrigin;
		Tween* m_UIAnimationTween;

		String m_currentSpeechString;

		AudioStreamPlayer* m_audioPlayer;
		Ref<AudioStream> m_scrollPlayer;
};
}