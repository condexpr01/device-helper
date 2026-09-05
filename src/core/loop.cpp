#include "core.hpp"
#include "gl-debug.hpp"

//#include "gl-frame-dump.hpp"

/*
struct ubo_struct{
	glm::mat4 mvp;       //64[16]
}ubo_data;
*/


class prepare{
	public:
		void set_up_current_ctx(core::sdl_event_ctx &ctx);
		void set_up_gl_program(core::sdl_event_ctx &ctx);
		void set_up_gl_capabilities(core::sdl_event_ctx &ctx);
		void set_up_gl_objects(core::sdl_event_ctx &ctx);

	public:
		void all(core::sdl_event_ctx &ctx){
			set_up_current_ctx(ctx);

			//set_up_gl_capabilities(ctx);
			//set_up_gl_objects(ctx);
			//set_up_gl_program(ctx);
		}
};

class frame{
	public:
		void imgui_window(core::sdl_event_ctx &ctx);

	public:
		void handle_events(core::sdl_event_ctx &ctx);
		void init_frame(core::sdl_event_ctx &ctx);
		void draw_scene(core::sdl_event_ctx &ctx);
		void build_ui(core::sdl_event_ctx &ctx);
		void draw_ui(core::sdl_event_ctx &ctx);
		void render_frame(core::sdl_event_ctx &ctx);

	//all
	public:
		void one_frame(core::sdl_event_ctx &ctx){
			handle_events(ctx);
			init_frame(ctx);

			build_ui(ctx);

			draw_scene(ctx);
			draw_ui(ctx);

			render_frame(ctx);
		}
};

//forward decl
void ColorfulStyle();
void page_style_content();
void page_io_content();
void page_sdl_audio_content(core::sdl_event_ctx &ctx);
void page_sdl_camera_content(core::sdl_event_ctx &ctx);
void page_sdl_event_content(core::sdl_event_ctx &ctx);
void page_sdl_hints_content(core::sdl_event_ctx &ctx);
void page_sdl_mics_content(core::sdl_event_ctx &ctx);
void page_sdl_hit_test_content(core::sdl_event_ctx &ctx);
void page_cmd_content(core::sdl_event_ctx &ctx);

void DrawCountCard(
		int64_t count,
		std::string title = "Colorful counter",
		std::string num_suffix = "",
		std::string label = "current times",
		ImVec2 size = {300,300},
		float rounding=12.f,
		float title_font_size = 20.f,
		float num_font_size = 56.f,
		float label_font_size = 16.f
		);

void prepare::set_up_gl_capabilities(core::sdl_event_ctx &ctx){

	//glEnable(GL_PRIMITIVE_RESTART_FIXED_INDEX);
	//glEnable(GL_PRIMITIVE_RESTART);
	//glPrimitiveRestartIndex(std::numeric_limits<GLuint>::max());

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_GEQUAL);
	glDepthMask(GL_TRUE);

	glEnable(GL_LINE_SMOOTH);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_MULTISAMPLE);

	//glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CCW);
	//glCullFace(GL_BACK);
}


void prepare::set_up_gl_program(core::sdl_event_ctx &ctx){
}

void prepare::set_up_gl_objects(core::sdl_event_ctx &ctx){
}

void prepare::set_up_current_ctx(core::sdl_event_ctx &ctx){
	if(!SDL_GL_MakeCurrent(ctx.swm.window,ctx.sgcm.gl_ctx)){
		ctx.status = false;
		ctx.reason = "[prepare::set_up_current_ctx]SDL_GL_MakeCurrent";
		return;
	}

	if(!SDL_GL_SetSwapInterval(1)){
		ctx.status = false;
		ctx.reason = "[prepare::set_up_current_ctx]SDL_GL_SetSwapInterval";
		return;
	}

	ctx.status = true;
	ctx.running = true;
}


void frame::handle_events(core::sdl_event_ctx &ctx){

	while(SDL_PollEvent(&ctx.e)){
		if (ctx.e.type == SDL_EVENT_QUIT){ctx.running = false;}

		do{//change running state in main window
			if (ctx.e.window.windowID != SDL_GetWindowID(ctx.swm.window))break;

			if (ctx.e.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED){ctx.running = false;}
			if (ctx.e.type == SDL_EVENT_KEY_DOWN && ctx.e.key.key == SDLK_ESCAPE){ctx.running = false;}
		}while(false);

		ImGui_ImplSDL3_ProcessEvent(&ctx.e);

		//update only if page is PAGE_SDL_EVENT
		if(ctx.page == core::PAGE_SDL_EVENT){
			core::log_sdl_event(ctx.e, ctx.e_tbuf);
			//core::log_sdl_event_description(ctx.e, ctx.e_tbuf);
		}
	}

	if (ctx.window_draggable && ctx.keyboard.is_pressed(SDL_SCANCODE_SPACE)){
		ctx.window_draggable = false;
	}

	#if 0
	if(ctx.keyboard.is_pressed(SDL_SCANCODE_F)){
		ctx.camera.translate(0.000f,-0.001f,0.f);
	}

	if(ctx.keyboard.is_pressed(SDL_SCANCODE_R)){
		ctx.camera.translate(0.000f,0.001f,0.f);
	}

	if(ctx.keyboard.is_pressed(SDL_SCANCODE_SPACE)){
		ctx.camera.translate(0.000f,0.001f,0.f);
	}

	if(ctx.keyboard.is_pressed(SDL_SCANCODE_W)){
		ctx.camera.translate(0.f,0.f,0.001f);
	}

	if(ctx.keyboard.is_pressed(SDL_SCANCODE_S)){
		ctx.camera.translate(0.f,0.f,-0.001f);
	}

	if(ctx.keyboard.is_pressed(SDL_SCANCODE_A)){
		ctx.camera.translate(-0.001f,0.f,0.f);
	}

	if(ctx.keyboard.is_pressed(SDL_SCANCODE_D)){
		ctx.camera.translate(0.001f,0.f,0.f);
	}

	if(ctx.keyboard.is_pressed(SDL_SCANCODE_H)){
		ctx.camera.auto_roll_rotate(-0.1f,0);
	}

	if(ctx.keyboard.is_pressed(SDL_SCANCODE_J)){
		ctx.camera.auto_roll_rotate(0,-0.1f);
	}

	if(ctx.keyboard.is_pressed(SDL_SCANCODE_K)){
		ctx.camera.auto_roll_rotate(0,0.1f);
	}

	if(ctx.keyboard.is_pressed(SDL_SCANCODE_L)){
		ctx.camera.auto_roll_rotate(0.1f,0);
	}
	#endif

}


void frame::draw_scene(core::sdl_event_ctx &ctx){
	//debug
	core::debug_error();
}

void frame::imgui_window(core::sdl_event_ctx &ctx){
	IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing Dear ImGui context.");
	IMGUI_CHECKVERSION();

	ImGui::NewFrame();

	//style
	static bool once = true;
	if(once){
		ColorfulStyle();
		once = false;
	}

	//drawlist and tips
	#if 1
	ImGuiViewport *viewport = ImGui::GetMainViewport();
	ImDrawList* foreground_drawlist = ImGui::GetForegroundDrawList();

	(*foreground_drawlist).AddText(ImVec2{0,0},IM_COL32(0x39,0xc5,0xbb,0xff),"condexpr01");

	if(ctx.window_draggable){
		ImGuiWindowFlags tips_window_flags{};
		tips_window_flags |= ImGuiWindowFlags_NoMove;
		tips_window_flags |= ImGuiWindowFlags_NoResize;
		tips_window_flags |= ImGuiWindowFlags_NoDecoration;
		tips_window_flags |= ImGuiWindowFlags_NoTitleBar;

		ImGui::SetNextWindowPos((*viewport).GetCenter(),0,ImVec2{0.5,0.5});
		if(ImGui::Begin("Tips",nullptr,tips_window_flags)){
			ImGui::Text("PRESS SPACE TO STOP");
		}
		ImGui::End();
	}
	#endif

	#if 1

	ImGui::SetNextWindowPos((*viewport).Pos);
	ImGui::SetNextWindowSize((*viewport).Size);

	ImGuiWindowFlags main_window_flags{};
	main_window_flags |= ImGuiWindowFlags_NoMove;
	main_window_flags |= ImGuiWindowFlags_NoResize;
	main_window_flags |= ImGuiWindowFlags_NoDecoration;
	main_window_flags |= ImGuiWindowFlags_NoTitleBar;

	if(ImGui::Begin("##main window",nullptr,main_window_flags)){
		
		ImVec2 cra = ImGui::GetContentRegionAvail();

		ImVec2 left_size = ImVec2{cra.x * ctx.leftw_pct, cra.y * (1.f - ctx.bottomh_pct)};
		ImVec2 right_size = ImVec2{cra.x * (1.f - ctx.leftw_pct), cra.y * (1.f - ctx.bottomh_pct)};
		ImVec2 bottom_size = ImVec2{cra.x, cra.y * ctx.bottomh_pct};

		//left side
		if(ImGui::BeginChild("left",left_size,ImGuiChildFlags_Borders)){
			cra = ImGui::GetContentRegionAvail();
			ImVec2 button_size = ImVec2{cra.x , cra.y/20.f};

			//drag window
			if(ImGui::Button("drag window",button_size)){
				ctx.window_draggable = true;
			}

			//add button func
			auto add_button = [&ctx,&button_size](const char *name, core::page_status page){
				bool pop = false;
				ImU32 color_selected = IM_COL32(0x39,0xc5,0xbb,0xff);
				if(ctx.page == page){
					ImGui::PushStyleColor(ImGuiCol_Button, color_selected);
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color_selected);
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, color_selected);
					pop = true;
				}

				if(ImGui::Button(name,button_size)){
					ctx.page = page;
				}

				if(pop){
					ImGui::PopStyleColor(3);
					pop = false;
				}
			};


			add_button("IO", core::PAGE_IO);
			add_button("Style", core::PAGE_STYLE);
			add_button("SDL_AUDIO", core::PAGE_SDL_AUDIO);
			add_button("SDL_CAMERA", core::PAGE_SDL_CAMERA);
			add_button("SDL_EVENT", core::PAGE_SDL_EVENT);
			add_button("SDL_HINTS", core::PAGE_SDL_HINTS);
			add_button("SDL_MICS", core::PAGE_SDL_MICS);
			add_button("Hit Test", core::PAGE_HIT_TEST);
			add_button("command", core::PAGE_CMD);
		}
		ImGui::EndChild();

		ImGui::SameLine(0.f,0.f);

		//right side
		if(ImGui::BeginChild("right",right_size,ImGuiChildFlags_Borders)){

			switch(ctx.page){
				case core::PAGE_IO:{
					page_io_content();
					break;
				}

				case core::PAGE_STYLE:{

					ImGui::SeparatorText("window");

					//window size
					const char* items[] = {"256x256","512x512", "1024x1024", "1920x1080"};
					static int current_item = 2;

					if (ImGui::Combo("window size", &current_item, items, IM_ARRAYSIZE(items))) {
						if (std::strcmp(items[current_item],"1024x1024") == 0){
							SDL_SetWindowSize(ctx.swm.window,1024,1024);
						}
						else if (std::strcmp(items[current_item],"256x256") == 0){
							SDL_SetWindowSize(ctx.swm.window,256,256);
						}
						else if (std::strcmp(items[current_item],"512x512") == 0){
							SDL_SetWindowSize(ctx.swm.window,512,512);
						}
						else if (std::strcmp(items[current_item],"1920x1080") == 0){
							SDL_SetWindowSize(ctx.swm.window,1920,1080);
						}
					}

					page_style_content();
					//ImGui::ShowStyleEditor();

					break;
				}

				case core::PAGE_SDL_AUDIO:{
					page_sdl_audio_content(ctx);
					break;
				}

				case core::PAGE_SDL_CAMERA:{
					page_sdl_camera_content(ctx);
					break;
				}

				case core::PAGE_SDL_EVENT:{
					page_sdl_event_content(ctx);
					break;
				}

				case core::PAGE_SDL_HINTS:{
					page_sdl_hints_content(ctx);
					break;
				}

				case core::PAGE_SDL_MICS:{
					page_sdl_mics_content(ctx);
					break;
				}

				case core::PAGE_HIT_TEST:{
					page_sdl_hit_test_content(ctx);
					break;
				}

				case core::PAGE_CMD:{
					page_cmd_content(ctx);
					break;
				}

			}

		}
		ImGui::EndChild();

		//bottom status bar
		if(ImGui::BeginChild("bottom",bottom_size,ImGuiChildFlags_Borders)){
			//version
			ImGui::Text("imgui version: %s",ImGui::GetVersion());
		}
		ImGui::EndChild();

	}

	ImGui::End();
	#endif

	ImGui::EndFrame();
}


void frame::build_ui(core::sdl_event_ctx &ctx){
	IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing Dear ImGui context!");
	IMGUI_CHECKVERSION();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL3_NewFrame();

	imgui_window(ctx);
}

void frame::draw_ui(core::sdl_event_ctx &ctx){
	IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing Dear ImGui context!");
	IMGUI_CHECKVERSION();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		SDL_GL_MakeCurrent(ctx.swm.window, ctx.sgcm.gl_ctx);
	}
}

void frame::init_frame(core::sdl_event_ctx &ctx){

	//clear
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClearDepth(0.f);
	glClearStencil(0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	#if 0
	int w,h;
	SDL_GetWindowSize(ctx.swm.window,&w,&h);
	ctx.projection = glm::perspective(glm::radians(45.f),(float)w/h,100.f,0.1f);
	#endif
}

void frame::render_frame(core::sdl_event_ctx &ctx){

	#if 1
		if(!SDL_GL_SwapWindow(ctx.swm.window)){
			ctx.running = false;
			ctx.status = false;
			ctx.reason = "[frame::render_frame]SDL_GL_SwapWindow";
			return;
		}

		//SDL_Delay(0);
		//ctx.running = false;
	#else
		glFinish();

		int w,h;
		SDL_GetWindowSize(ctx.swm.window,&w,&h);
		core::frame_dump_png(w,h,ctx.png_path);

		ctx.running = false;
	#endif

}

//call top in loop
void core::sdl_event_manager::loop() {
	prepare prepare;
	frame frame;

	prepare.all(ctx);

	while(ctx.running && ctx.status){
		frame.one_frame(ctx);
	}

}

void page_io_content(){
	ImGuiIO &io = ImGui::GetIO();

	ImGui::SeparatorText("Configuration");
	ImGui::SeparatorText("io.ConfigFlags");
	if (ImGui::BeginTable("io.ConfigFlags", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)){

		auto show_flags= [&io](const char *text, ImGuiConfigFlags flag){
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::Text("%s",text);
			ImGui::TableSetColumnIndex(1);
			ImGui::CheckboxFlags(text, &io.ConfigFlags, flag);
		};

		ImGui::TableSetupColumn("Name");
		ImGui::TableSetupColumn("Status");
		ImGui::TableHeadersRow();

		show_flags("ImGuiConfigFlags_None",ImGuiConfigFlags_None);
		show_flags("NavEnableKeyboard",ImGuiConfigFlags_NavEnableKeyboard);
		show_flags("NavEnableGamepad",ImGuiConfigFlags_NavEnableGamepad);
		show_flags("NoMouse",ImGuiConfigFlags_NoMouse);
		show_flags("NoMouseCursorChange",ImGuiConfigFlags_NoMouseCursorChange);
		show_flags("NoKeyboard",ImGuiConfigFlags_NoKeyboard);
		show_flags("DockingEnable",ImGuiConfigFlags_DockingEnable);
		show_flags("ViewportsEnable",ImGuiConfigFlags_ViewportsEnable);
		show_flags("IsSRGB",ImGuiConfigFlags_IsSRGB);
		show_flags("IsTouchScreen",ImGuiConfigFlags_IsTouchScreen);

		ImGui::EndTable();
	}

	ImGui::SeparatorText("io.BackendFlags");
	if (ImGui::BeginTable("BackendFlags", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)){

		auto show_flags= [&io](const char *text, ImGuiBackendFlags flag){
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::Text("%s",text);
			ImGui::TableSetColumnIndex(1);
			ImGui::CheckboxFlags(text, &io.BackendFlags, flag);
		};

		ImGui::TableSetupColumn("Name");
		ImGui::TableSetupColumn("Status");
		ImGui::TableHeadersRow();

		show_flags("ImGuiBackendFlags_None",ImGuiBackendFlags_None);
		show_flags("HasGamepad",ImGuiBackendFlags_HasGamepad);
		show_flags("HasMouseCursors",ImGuiBackendFlags_HasMouseCursors);
		show_flags("HasSetMousePos",ImGuiBackendFlags_HasSetMousePos);
		show_flags("RendererHasVtxOffset",ImGuiBackendFlags_RendererHasVtxOffset);
		show_flags("RendererHasTextures",ImGuiBackendFlags_RendererHasTextures);
		show_flags("RendererHasViewports",ImGuiBackendFlags_RendererHasViewports);
		show_flags("PlatformHasViewports",ImGuiBackendFlags_PlatformHasViewports);
		show_flags("HasMouseHoveredViewport",ImGuiBackendFlags_HasMouseHoveredViewport);
		show_flags("HasParentViewport",ImGuiBackendFlags_HasParentViewport);

		ImGui::EndTable();
	}


	ImGui::SeparatorText("io.Config.*");
	if (ImGui::BeginTable("io.Config.*", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)){

		auto show_flags= [&io](const char *text, bool *flag){
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::Text("%s",text);
			ImGui::TableSetColumnIndex(1);
			ImGui::Checkbox(text, flag);
		};

		ImGui::TableSetupColumn("Name");
		ImGui::TableSetupColumn("Status");
		ImGui::TableHeadersRow();

		show_flags("ConfigNavSwapGamepadButtons",&io.ConfigNavSwapGamepadButtons);
		show_flags("ConfigNavMoveSetMousePos",&io.ConfigNavMoveSetMousePos);
		show_flags("ConfigNavCaptureKeyboard",&io.ConfigNavCaptureKeyboard);
		show_flags("ConfigNavEscapeClearFocusItem",&io.ConfigNavEscapeClearFocusItem);
		show_flags("ConfigNavEscapeClearFocusWindow",&io.ConfigNavEscapeClearFocusWindow);
		show_flags("ConfigNavCursorVisibleAuto",&io.ConfigNavCursorVisibleAuto);
		show_flags("ConfigNavCursorVisibleAlways",&io.ConfigNavCursorVisibleAlways);
		show_flags("ConfigDockingNoSplit",&io.ConfigDockingNoSplit);
		show_flags("ConfigDockingNoDockingOver",&io.ConfigDockingNoDockingOver);
		show_flags("ConfigDockingWithShift",&io.ConfigDockingWithShift);
		show_flags("ConfigDockingAlwaysTabBar",&io.ConfigDockingAlwaysTabBar);
		show_flags("ConfigDockingTransparentPayload",&io.ConfigDockingTransparentPayload);
		show_flags("ConfigViewportsNoAutoMerge",&io.ConfigViewportsNoAutoMerge);
		show_flags("ConfigViewportsNoTaskBarIcon",&io.ConfigViewportsNoTaskBarIcon);
		show_flags("ConfigViewportsNoDecoration",&io.ConfigViewportsNoDecoration);
		show_flags("ConfigViewportsNoDefaultParent",&io.ConfigViewportsNoDefaultParent);
		show_flags("ConfigViewportsPlatformFocusSetsImGuiFocus",&io.ConfigViewportsPlatformFocusSetsImGuiFocus);
		show_flags("ConfigDpiScaleFonts",&io.ConfigDpiScaleFonts);
		show_flags("ConfigDpiScaleViewports",&io.ConfigDpiScaleViewports);
		show_flags("MouseDrawCursor",&io.MouseDrawCursor);
		show_flags("ConfigMacOSXBehaviors",&io.ConfigMacOSXBehaviors);
		show_flags("ConfigInputTrickleEventQueue",&io.ConfigInputTrickleEventQueue);
		show_flags("ConfigInputTextCursorBlink",&io.ConfigInputTextCursorBlink);
		show_flags("ConfigInputTextEnterKeepActive",&io.ConfigInputTextEnterKeepActive);
		show_flags("ConfigDragClickToInputText",&io.ConfigDragClickToInputText);
		show_flags("ConfigWindowsResizeFromEdges",&io.ConfigWindowsResizeFromEdges);
		show_flags("ConfigWindowsMoveFromTitleBarOnly",&io.ConfigWindowsMoveFromTitleBarOnly);
		show_flags("ConfigWindowsCopyContentsWithCtrlC",&io.ConfigWindowsCopyContentsWithCtrlC);
		show_flags("ConfigScrollbarScrollByPage",&io.ConfigScrollbarScrollByPage);

		ImGui::EndTable();
	}

	ImGui::SeparatorText("io config values");
	ImGui::Text("io.DisplaySize: %.2fx%.2f",io.DisplaySize.x,io.DisplaySize.y);
	ImGui::Text("io.FramebufferScale: %.2fx%.2f",io.DisplayFramebufferScale.x,io.DisplayFramebufferScale.y);
	ImGui::Text("io.DeltaTime: %f",io.DeltaTime);
	ImGui::Text("io.IniSavingRate: %fs",io.IniSavingRate);
	ImGui::Text("io.IniFilename: %s",io.IniFilename);
	ImGui::Text("io.LogFilename: %s",io.LogFilename);
	ImGui::Text("io.Fonts : %p",io.Fonts);
	ImGui::Text("io.FontDefault : %p",io.FontDefault);
	ImGui::Checkbox("io.FontAllowUsersScaling",&io.FontAllowUserScaling);

	ImGui::DragFloat("ConfigMemoryCompactTimer",&io.ConfigMemoryCompactTimer);
	ImGui::DragFloat("MouseDoubleClickTime",&io.MouseDoubleClickTime);
	ImGui::DragFloat("MouseDoubleClickMaxDist",&io.MouseDoubleClickMaxDist);
	ImGui::DragFloat("MouseDragThreshold",&io.MouseDragThreshold);
	ImGui::DragFloat("KeyRepeatDelay",&io.KeyRepeatDelay);
	ImGui::DragFloat("KeyRepeatRate",&io.KeyRepeatRate);


	ImGui::SeparatorText("Debug");
	if (ImGui::BeginTable("debug config", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)){

		auto show_flags= [&io](const char *text, bool *flag){
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::Text("%s",text);
			ImGui::TableSetColumnIndex(1);
			ImGui::Checkbox(text, flag);
		};

		ImGui::TableSetupColumn("Name");
		ImGui::TableSetupColumn("Status");
		ImGui::TableHeadersRow();

		show_flags("ConfigErrorRecovery",&io.ConfigErrorRecovery);
		show_flags("ConfigErrorRecoveryEnableAssert",&io.ConfigErrorRecoveryEnableAssert);
		show_flags("ConfigErrorRecoveryEnableDebugLog",&io.ConfigErrorRecoveryEnableDebugLog);
		show_flags("ConfigErrorRecoveryEnableTooltip",&io.ConfigErrorRecoveryEnableTooltip);
		show_flags("ConfigDebugIsDebuggerPresent",&io.ConfigDebugIsDebuggerPresent);
		show_flags("ConfigDebugHighlightIdConflicts",&io.ConfigDebugHighlightIdConflicts);
		show_flags("ConfigDebugHighlightIdConflictsShowItemPicker",&io.ConfigDebugHighlightIdConflictsShowItemPicker);
		show_flags("ConfigDebugBeginReturnValueOnce",&io.ConfigDebugBeginReturnValueOnce);
		show_flags("ConfigDebugBeginReturnValueLoop",&io.ConfigDebugBeginReturnValueLoop);
		show_flags("ConfigDebugIgnoreFocusLoss",&io.ConfigDebugIgnoreFocusLoss);
		show_flags("ConfigDebugIniSettings",&io.ConfigDebugIniSettings);

		ImGui::EndTable();
	}


	if (ImGui::BeginTable("want", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)){

		auto show_flags= [&io](const char *text, bool *flag){
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::Text("%s",text);
			ImGui::TableSetColumnIndex(1);
			ImGui::Checkbox(text, flag);
		};

		ImGui::TableSetupColumn("Name");
		ImGui::TableSetupColumn("Status");
		ImGui::TableHeadersRow();

		show_flags("WantCaptureMouse",&io.WantCaptureMouse);
		show_flags("WantCaptureMouseUnlessPopupClose",&io.WantCaptureMouseUnlessPopupClose);
		show_flags("WantCaptureKeyboard",&io.WantCaptureKeyboard);
		show_flags("WantTextInput",&io.WantTextInput);
		show_flags("WantSetMousePos",&io.WantSetMousePos);
		show_flags("WantSaveIniSettings",&io.WantSaveIniSettings);
		show_flags("NavActive",&io.NavActive);
		show_flags("NavVisible",&io.NavVisible);

		ImGui::EndTable();
	}

	ImGui::Text("io.Framerate %.2f",io.Framerate);
	ImGui::Text("io.BackendPlatformName: %s",io.BackendPlatformName);
	ImGui::Text("io.BackendRendererName: %s",io.BackendRendererName);

	ImGui::Text("io.MetricsRenderVertices: %d",io.MetricsRenderVertices);
	ImGui::Text("io.MetricsRenderIndices: %d",io.MetricsRenderIndices);
	ImGui::Text("io.MetricsRenderWindows: %d",io.MetricsRenderWindows);
	ImGui::Text("io.MetricsActiveWindows: %d",io.MetricsActiveWindows);

	ImGui::Text("io.MouseDelta: %f %f",io.MouseDelta.x,io.MouseDelta.y);
	ImGui::Text("io.Ctx: %p",io.Ctx);
	ImGui::Text("io.MousePosPrev: %f %f",io.MousePosPrev.x,io.MousePosPrev.y);
	ImGui::Text("io.MousePos: %f %f",io.MousePos.x,io.MousePos.y);
	ImGui::Text("io.MouseDown: %d %d %d %d %d",
			io.MouseDown[0],io.MouseDown[1],io.MouseDown[2],io.MouseDown[3],io.MouseDown[4]);
	ImGui::Text("io.MouseWheel: %f",io.MouseWheel);
	ImGui::Text("io.MouseWheelH: %f",io.MouseWheelH);
	ImGui::Text("io.MouseSource: %d",io.MouseSource);
	ImGui::Text("io.MouseHoveredViewport: %d",io.MouseHoveredViewport);

	ImGui::Text("io.KeyCtrl: %d",io.KeyCtrl);
	ImGui::Text("io.KeyShift: %d",io.KeyShift);
	ImGui::Text("io.KeyAlt: %d",io.KeyAlt);
	ImGui::Text("io.KeySuper: %d",io.KeySuper);
	ImGui::Text("io.KeyMods: %d",io.KeyMods);

	for(int i=0;i<5;i++){
		ImGui::Text("io.MouseClickedPos[%d]: %f %f",
				i,io.MouseClickedPos[i].x,io.MouseClickedPos[i].y);
	}

	for(int i=0;i<5;i++){
		ImGui::Text("io.MouseClickedTime[%d]: %lf",
				i,io.MouseClickedTime[i]);
	}

	for(int i=0;i<5;i++){
		ImGui::Text("io.MouseClicked[%d]: %d",
				i,io.MouseClicked[i]);
	}

	for(int i=0;i<5;i++){
		ImGui::Text("io.MouseDoubleClicked[%d]: %d",
				i,io.MouseDoubleClicked[i]);
	}

	for(int i=0;i<5;i++){
		ImGui::Text("io.MouseClickedCount[%d]: %d",
				i,io.MouseClickedCount[i]);
	}

	for(int i=0;i<5;i++){
		ImGui::Text("io.MouseClickedLastCount[%d]: %d",
				i,io.MouseClickedLastCount[i]);
	}

	for(int i=0;i<5;i++){
		ImGui::Text("io.MouseReleased[%d]: %d",
				i,io.MouseReleased[i]);
	}

	for(int i=0;i<5;i++){
		ImGui::Text("io.MouseReleasedTime[%d]: %lf",
				i,io.MouseReleasedTime[i]);
	}

	for(int i=0;i<5;i++){
		ImGui::Text("io.MouseDownOwned[%d]: %d",
				i,io.MouseDownOwned[i]);
	}

	for(int i=0;i<5;i++){
		ImGui::Text("io.MouseDownOwnedUnlessPopupClose[%d]: %d",
				i,io.MouseDownOwnedUnlessPopupClose[i]);
	}

	for(int i=0;i<5;i++){
		ImGui::Text("io.MouseDownDuration[%d]: %f",
				i,io.MouseDownDuration[i]);
	}

	for(int i=0;i<5;i++){
		ImGui::Text("io.MouseDownDurationPrev[%d]: %f",
				i,io.MouseDownDurationPrev[i]);
	}

	ImGui::Text("io.MouseCtrlLeftAsRightClick: %d",io.MouseCtrlLeftAsRightClick);
	ImGui::Text("io.MouseWheelRequestAxisSwap: %d",io.MouseWheelRequestAxisSwap);

	for(int i=0;i<5;i++){
		ImGui::Text("io.MouseDragMaxDistanceAbs[%d]: %f %f",
				i,io.MouseDragMaxDistanceAbs[i].x,io.MouseDragMaxDistanceAbs[i].y);
	}

	for(int i=0;i<5;i++){
		ImGui::Text("io.MouseDragMaxDistanceSqr[%d]: %f",
				i,io.MouseDragMaxDistanceSqr[i]);
	}

	ImGui::Text("io.PenPressure: %f",io.PenPressure);
	ImGui::Text("io.AppFocusLost: %d",io.AppFocusLost);
	ImGui::Text("io.AppAcceptingEvents: %d",io.AppAcceptingEvents);
	ImGui::Text("io.InputQueueSurrogate: %#x",io.InputQueueSurrogate);

	for(auto &&e : io.InputQueueCharacters){
		ImGui::Text("io.InputQueueCharacters: %#x",e);
	}
}

void page_style_content(){
	ImGuiStyle &style = ImGui::GetStyle();
	ImGuiIO& io = ImGui::GetIO();
	for (ImFont* font : (*io.Fonts).Fonts) {
		const char* font_name = (*font).GetDebugName(); 
		ImGui::Text("FontId: %d ,FontName: %s",(*font).FontId, font_name);
	}

	ImGui::SeparatorText("Values");
	ImGui::DragFloat("style.FontSizeBase",&style.FontSizeBase);
	ImGui::DragFloat("style.FontScaleMain",&style.FontScaleMain,0.01f);
	ImGui::DragFloat("style.FontScaleDpi",&style.FontScaleDpi,0.01f);
	ImGui::DragFloat("style.Alpha",&style.Alpha,0.01f,0.1f,1.f);
	ImGui::DragFloat("style.DisabledAlpha",&style.DisabledAlpha,0.01f,0.1f,1.f);
	ImGui::DragFloat("style.WindowPadding.x",&style.WindowPadding.x);
	ImGui::DragFloat("style.WindowPadding.y",&style.WindowPadding.y);
	ImGui::DragFloat("style.WindowRounding",&style.WindowRounding);
	ImGui::DragFloat("style.WindowBorderSize",&style.WindowBorderSize);
	ImGui::DragFloat("style.WindowBorderHoverPadding",
			&style.WindowBorderHoverPadding,
			0.1f,0.1f,10000.f);
	ImGui::DragFloat("style.WindowMinSize.x",&style.WindowMinSize.x,1.f,1.f,10000.f);
	ImGui::DragFloat("style.WindowMinSize.y",&style.WindowMinSize.y,1.f,1.f,10000.f);
	ImGui::DragFloat("style.WindowTitleAlign.x",&style.WindowTitleAlign.x);
	ImGui::DragFloat("style.WindowTitleAlign.y",&style.WindowTitleAlign.y);

	ImGui::Text("style.WindowMenuButtonPosition");
	ImGui::PushID("style.WindowMenuButtonPosition ImGuiDir");
	ImGui::RadioButton("ImGuiDir_None",  (int*)&style.WindowMenuButtonPosition, ImGuiDir_None);
	ImGui::SameLine();
	ImGui::RadioButton("ImGuiDir_Left",  (int*)&style.WindowMenuButtonPosition, ImGuiDir_Left);
	ImGui::SameLine();
	ImGui::RadioButton("ImGuiDir_Right", (int*)&style.WindowMenuButtonPosition, ImGuiDir_Right);
	ImGui::SameLine();
	ImGui::RadioButton("ImGuiDir_Up", false);
	ImGui::SameLine();
	ImGui::RadioButton("ImGuiDir_Down", false);
	ImGui::PopID();

	ImGui::DragFloat("style.ChildRounding",&style.ChildRounding,0.1f);
	ImGui::DragFloat("style.ChildBorderSize",&style.ChildBorderSize,0.1f);
	ImGui::DragFloat("style.PopupRounding",&style.PopupRounding,0.1f);
	ImGui::DragFloat("style.PopupBorderSize",&style.PopupBorderSize,0.1f);

	ImGui::DragFloat("style.FramePadding.x",&style.FramePadding.x,0.1f);
	ImGui::DragFloat("style.FramePadding.y",&style.FramePadding.y,0.1f);

	ImGui::DragFloat("style.FrameRounding",&style.FrameRounding,0.1f);
	ImGui::DragFloat("style.FrameBorderSize",&style.FrameBorderSize,0.1f);

	ImGui::DragFloat("style.ItemSpacing.x",&style.ItemSpacing.x,0.1f);
	ImGui::DragFloat("style.ItemSpacing.y",&style.ItemSpacing.y,0.1f);

	ImGui::DragFloat("style.ItemInnerSpacing.x",&style.ItemInnerSpacing.x,0.1f);
	ImGui::DragFloat("style.ItemInnerSpacing.y",&style.ItemInnerSpacing.y,0.1f);

	ImGui::DragFloat("style.CellPadding.x",&style.CellPadding.x,0.1f);
	ImGui::DragFloat("style.CellPadding.y",&style.CellPadding.y,0.1f);

	ImGui::DragFloat("style.TouchExtraPadding.x",&style.TouchExtraPadding.x,0.1f);
	ImGui::DragFloat("style.TouchExtraPadding.y",&style.TouchExtraPadding.y,0.1f);

	ImGui::DragFloat("style.IndentSpacing",&style.IndentSpacing,0.1f);
	ImGui::DragFloat("style.ColumnsMinSpacing",&style.ColumnsMinSpacing,0.1f);
	ImGui::DragFloat("style.ScrollbarSize",&style.ScrollbarSize,0.1f);
	ImGui::DragFloat("style.ScrollbarRounding",&style.ScrollbarRounding,0.1f);
	ImGui::DragFloat("style.ScrollbarPadding",&style.ScrollbarPadding,0.1f);
	ImGui::DragFloat("style.GrabMinSize",&style.GrabMinSize,0.1f);
	ImGui::DragFloat("style.GrabRounding",&style.GrabRounding,0.1f);
	ImGui::DragFloat("style.LogSliderDeadzone",&style.LogSliderDeadzone,0.1f);
	ImGui::DragFloat("style.ImageRounding",&style.ImageRounding,0.1f);
	ImGui::DragFloat("style.ImageBorderSize",&style.ImageBorderSize,0.1f);
	ImGui::DragFloat("style.TabRounding",&style.TabRounding,0.1f);
	ImGui::DragFloat("style.TabBorderSize",&style.TabBorderSize,0.1f);
	ImGui::DragFloat("style.TabMinWidthBase",&style.TabMinWidthBase,0.1f);
	ImGui::DragFloat("style.TabMinWidthShrink",&style.TabMinWidthShrink,0.1f);
	ImGui::DragFloat("style.TabCloseButtonMinWidthSelected",&style.TabCloseButtonMinWidthSelected,0.1f);
	ImGui::DragFloat("style.TabCloseButtonMinWidthUnselected",&style.TabCloseButtonMinWidthUnselected,0.1f);
	ImGui::DragFloat("style.TabBarBorderSize",&style.TabBarBorderSize,0.1f);
	ImGui::DragFloat("style.TabBarOverlineSize",&style.TabBarOverlineSize,0.1f);
	ImGui::DragFloat("style.TableAngledHeadersAngle",&style.TableAngledHeadersAngle,0.1f);

	ImGui::DragFloat("style.TableAngledHeadersTextAlign.x",&style.TableAngledHeadersTextAlign.x,0.1f);
	ImGui::DragFloat("style.TableAngledHeadersTextAlign.y",&style.TableAngledHeadersTextAlign.y,0.1f);

	ImGui::Text("style.TreeLinesFlags: %#x",style.TreeLinesFlags);

	ImGui::DragFloat("style.TreeLinesSize",&style.TreeLinesSize,0.1f);
	ImGui::DragFloat("style.TreeLinesRounding",&style.TreeLinesRounding,0.1f);
	ImGui::DragFloat("style.MenuItemRounding",&style.MenuItemRounding,0.1f);
	ImGui::DragFloat("style.SelectableRounding",&style.SelectableRounding,0.1f);
	ImGui::DragFloat("style.DragDropTargetRounding",&style.DragDropTargetRounding,0.1f);
	ImGui::DragFloat("style.DragDropTargetBorderSize",&style.DragDropTargetBorderSize,0.1f);
	ImGui::DragFloat("style.DragDropTargetPadding",&style.DragDropTargetPadding,0.1f);
	ImGui::DragFloat("style.ColorMarkerSize",&style.ColorMarkerSize,0.1f);

	ImGui::Text("style.ColorButtonPosition");

	ImGui::PushID("style.ColorButtonPosition ImGuiDir");
	ImGui::RadioButton("ImGuiDir_None", false);
	ImGui::SameLine();
	ImGui::RadioButton("ImGuiDir_Left",  (int*)&style.ColorButtonPosition, ImGuiDir_Left);
	ImGui::SameLine();
	ImGui::RadioButton("ImGuiDir_Right", (int*)&style.ColorButtonPosition, ImGuiDir_Right);
	ImGui::SameLine();
	ImGui::RadioButton("ImGuiDir_Up", false);
	ImGui::SameLine();
	ImGui::RadioButton("ImGuiDir_Down", false);
	ImGui::PopID();

	ImGui::DragFloat("style.ButtonTextAlign.x",&style.ButtonTextAlign.x,0.1f);
	ImGui::DragFloat("style.ButtonTextAlign.y",&style.ButtonTextAlign.y,0.1f);

	ImGui::DragFloat("style.SelectableTextAlign.x",&style.SelectableTextAlign.x,0.1f);
	ImGui::DragFloat("style.SelectableTextAlign.y",&style.SelectableTextAlign.y,0.1f);

	ImGui::DragFloat("style.InputTextCursorSize",&style.InputTextCursorSize,0.1f);
	ImGui::DragFloat("style.SeparatorSize",&style.SeparatorSize,0.1f);
	ImGui::DragFloat("style.SeparatorTextBorderSize",&style.SeparatorTextBorderSize,0.1f);

	ImGui::DragFloat("style.SeparatorTextAlign.x",&style.SeparatorTextAlign.x,0.1f);
	ImGui::DragFloat("style.SeparatorTextAlign.y",&style.SeparatorTextAlign.y,0.1f);

	ImGui::DragFloat("style.SeparatorTextPadding.x",&style.SeparatorTextPadding.x,0.1f);
	ImGui::DragFloat("style.SeparatorTextPadding.y",&style.SeparatorTextPadding.y,0.1f);

	ImGui::DragFloat("style.DisplayWindowPadding.x",&style.DisplayWindowPadding.x,0.1f);
	ImGui::DragFloat("style.DisplayWindowPadding.y",&style.DisplayWindowPadding.y,0.1f);

	ImGui::DragFloat("style.DisplaySafeAreaPadding.x",&style.DisplaySafeAreaPadding.x,0.1f);
	ImGui::DragFloat("style.DisplaySafeAreaPadding.y",&style.DisplaySafeAreaPadding.y,0.1f);

	ImGui::Checkbox("style.DockingNodeHasCloseButton",&style.DockingNodeHasCloseButton);

	ImGui::DragFloat("style.DockingSeparatorSize",&style.DockingSeparatorSize,0.1f);
	ImGui::DragFloat("style.MouseCursorScale",&style.MouseCursorScale,0.1f);

	ImGui::Checkbox("style.AntiAliasedLines",&style.AntiAliasedLines);
	ImGui::Checkbox("style.AntiAliasedLinesUseTex",&style.AntiAliasedLinesUseTex);
	ImGui::Checkbox("style.AntiAliasedFill",&style.AntiAliasedFill);

	ImGui::DragFloat("style.CurveTessellationTol",&style.CurveTessellationTol,0.1f);
	ImGui::DragFloat("style.CircleTessellationMaxError",&style.CircleTessellationMaxError,0.1f,0.1f,10000.f);

	ImGui::DragFloat("style.HoverStationaryDelay",&style.HoverStationaryDelay,0.1f);
	ImGui::DragFloat("style.HoverDelayShort",&style.HoverDelayShort,0.1f);
	ImGui::DragFloat("style.HoverDelayNormal",&style.HoverDelayNormal,0.1f);

	ImGui::Text("style.HoverFlagsForTooltipMouse: %#x",style.HoverFlagsForTooltipMouse);
	ImGui::Text("style.HoverFlagsForTooltipNav: %#x",style.HoverFlagsForTooltipNav);

	ImGui::SeparatorText("Colors");
	for (int i = 0; i < ImGuiCol_COUNT; i++) {
		ImGui::ColorEdit4(ImGui::GetStyleColorName(i), &style.Colors[i].x, ImGuiColorEditFlags_DisplayHex);
	}

	ImGui::SeparatorText("Copyable area");
	auto dump_style = [&style]() -> std::string{
		std::stringstream ss;
		ss << std::fixed << std::setprecision(4);

		ss << "ImGuiStyle& style = ImGui::GetStyle();\n\n";

		auto dump_float = [&ss](const char* name, float value) {
			ss << "style." << name << " = " << value << "f;\n";
		};
		auto dump_bool  = [&ss](const char* name, bool value) {
			ss << "style." << name << " = " << (value ? "true" : "false") << ";\n";
		};
		auto dump_vec2  = [&ss](const char* name, const ImVec2& value) {
			ss << "style." << name << " = ImVec2(" << value.x << "f, " << value.y << "f);\n";
		};
		auto dump_enum_int = [&ss](const char* name, int value, const char* enum_name = "") {
			if (enum_name && enum_name[0])
				ss << "style." << name << " = (" << enum_name << ")" << value << ";\n";
			else
				ss << "style." << name << " = " << value << ";\n";
		};

		ss << "// ---------- Font scaling ----------\n";
		dump_float("FontSizeBase", style.FontSizeBase);
		dump_float("FontScaleMain", style.FontScaleMain);
		dump_float("FontScaleDpi", style.FontScaleDpi);

		ss << "\n// ---------- Alpha ----------\n";
		dump_float("Alpha", style.Alpha);
		dump_float("DisabledAlpha", style.DisabledAlpha);

		ss << "\n// ---------- Window ----------\n";
		dump_vec2("WindowPadding", style.WindowPadding);
		dump_float("WindowRounding", style.WindowRounding);
		dump_float("WindowBorderSize", style.WindowBorderSize);
		dump_float("WindowBorderHoverPadding", style.WindowBorderHoverPadding);
		dump_vec2("WindowMinSize", style.WindowMinSize);
		dump_vec2("WindowTitleAlign", style.WindowTitleAlign);
		dump_enum_int("WindowMenuButtonPosition",
				(int)style.WindowMenuButtonPosition, "ImGuiDir");

		ss << "\n// ---------- Child & Popup ----------\n";
		dump_float("ChildRounding", style.ChildRounding);
		dump_float("ChildBorderSize", style.ChildBorderSize);
		dump_float("PopupRounding", style.PopupRounding);
		dump_float("PopupBorderSize", style.PopupBorderSize);

		ss << "\n// ---------- Frame & Item ----------\n";
		dump_vec2("FramePadding", style.FramePadding);
		dump_float("FrameRounding", style.FrameRounding);
		dump_float("FrameBorderSize", style.FrameBorderSize);
		dump_vec2("ItemSpacing", style.ItemSpacing);
		dump_vec2("ItemInnerSpacing", style.ItemInnerSpacing);
		dump_vec2("CellPadding", style.CellPadding);
		dump_vec2("TouchExtraPadding", style.TouchExtraPadding);
		dump_float("IndentSpacing", style.IndentSpacing);
		dump_float("ColumnsMinSpacing", style.ColumnsMinSpacing);

		ss << "\n// ---------- Scrollbar ----------\n";
		dump_float("ScrollbarSize", style.ScrollbarSize);
		dump_float("ScrollbarRounding", style.ScrollbarRounding);
		dump_float("ScrollbarPadding", style.ScrollbarPadding);

		ss << "\n// ---------- Grab ----------\n";
		dump_float("GrabMinSize", style.GrabMinSize);
		dump_float("GrabRounding", style.GrabRounding);
		dump_float("LogSliderDeadzone", style.LogSliderDeadzone);

		ss << "\n// ---------- Image ----------\n";
		dump_float("ImageRounding", style.ImageRounding);
		dump_float("ImageBorderSize", style.ImageBorderSize);

		ss << "\n// ---------- Tab ----------\n";
		dump_float("TabRounding", style.TabRounding);
		dump_float("TabBorderSize", style.TabBorderSize);
		dump_float("TabMinWidthBase", style.TabMinWidthBase);
		dump_float("TabMinWidthShrink", style.TabMinWidthShrink);
		dump_float("TabCloseButtonMinWidthSelected", style.TabCloseButtonMinWidthSelected);
		dump_float("TabCloseButtonMinWidthUnselected", style.TabCloseButtonMinWidthUnselected);
		dump_float("TabBarBorderSize", style.TabBarBorderSize);
		dump_float("TabBarOverlineSize", style.TabBarOverlineSize);

		ss << "\n// ---------- Table ----------\n";
		dump_float("TableAngledHeadersAngle", style.TableAngledHeadersAngle);
		dump_vec2("TableAngledHeadersTextAlign", style.TableAngledHeadersTextAlign);

		ss << "\n// ---------- TreeLines ----------\n";
		dump_enum_int("TreeLinesFlags", (int)style.TreeLinesFlags, "ImGuiTreeNodeFlags");
		dump_float("TreeLinesSize", style.TreeLinesSize);
		dump_float("TreeLinesRounding", style.TreeLinesRounding);

		ss << "\n// ---------- MenuItem & Selectable ----------\n";
		dump_float("MenuItemRounding", style.MenuItemRounding);
		dump_float("SelectableRounding", style.SelectableRounding);
		dump_float("DragDropTargetRounding", style.DragDropTargetRounding);
		dump_float("DragDropTargetBorderSize", style.DragDropTargetBorderSize);
		dump_float("DragDropTargetPadding", style.DragDropTargetPadding);
		dump_float("ColorMarkerSize", style.ColorMarkerSize);
		dump_enum_int("ColorButtonPosition", (int)style.ColorButtonPosition, "ImGuiDir");
		dump_vec2("ButtonTextAlign", style.ButtonTextAlign);
		dump_vec2("SelectableTextAlign", style.SelectableTextAlign);
		dump_float("InputTextCursorSize", style.InputTextCursorSize);

		ss << "\n// ---------- Separator ----------\n";
		dump_float("SeparatorSize", style.SeparatorSize);
		dump_float("SeparatorTextBorderSize", style.SeparatorTextBorderSize);
		dump_vec2("SeparatorTextAlign", style.SeparatorTextAlign);
		dump_vec2("SeparatorTextPadding", style.SeparatorTextPadding);

		ss << "\n// ---------- Display ----------\n";
		dump_vec2("DisplayWindowPadding", style.DisplayWindowPadding);
		dump_vec2("DisplaySafeAreaPadding", style.DisplaySafeAreaPadding);

		ss << "\n// ---------- Docking ----------\n";
		dump_bool("DockingNodeHasCloseButton", style.DockingNodeHasCloseButton);
		dump_float("DockingSeparatorSize", style.DockingSeparatorSize);

		ss << "\n// ---------- Mouse Cursor ----------\n";
		dump_float("MouseCursorScale", style.MouseCursorScale);

		ss << "\n// ---------- Anti-Aliasing ----------\n";
		dump_bool("AntiAliasedLines", style.AntiAliasedLines);
		dump_bool("AntiAliasedLinesUseTex", style.AntiAliasedLinesUseTex);
		dump_bool("AntiAliasedFill", style.AntiAliasedFill);

		ss << "\n// ---------- Tessellation ----------\n";
		dump_float("CurveTessellationTol", style.CurveTessellationTol);
		dump_float("CircleTessellationMaxError", style.CircleTessellationMaxError);

		ss << "\n// ---------- Colors ----------\n";
		for (int i = 0; i < ImGuiCol_COUNT; ++i) {
			const ImVec4& c = style.Colors[i];
			ss << "style.Colors[ImGuiCol_" << ImGui::GetStyleColorName(i) << "] = ImVec4("
				<< c.x << "f, " << c.y << "f, " << c.z << "f, " << c.w << "f);\n";
		}

		ss << "\n// ---------- Hover Behaviors ----------\n";
		dump_float("HoverStationaryDelay", style.HoverStationaryDelay);
		dump_float("HoverDelayShort", style.HoverDelayShort);
		dump_float("HoverDelayNormal", style.HoverDelayNormal);
		dump_enum_int("HoverFlagsForTooltipMouse",
				(int)style.HoverFlagsForTooltipMouse, "ImGuiHoveredFlags");
		dump_enum_int("HoverFlagsForTooltipNav",
				(int)style.HoverFlagsForTooltipNav, "ImGuiHoveredFlags");
		return ss.str();
	};

	static std::string s{};

	if(ImGui::Button("copy")){
		ImGui::SetClipboardText(s.c_str());
	}
	ImGui::SameLine();
	if(ImGui::Button("dump")){
		s = dump_style();
		s.push_back('\0');
		ImGui::SetClipboardText(s.c_str());
	}


	ImGui::InputTextMultiline("##copyable text",s.data(),s.size(),
			ImVec2{ImGui::GetContentRegionAvail().x,ImGui::GetTextLineHeight() * 128},
			ImGuiInputTextFlags_ReadOnly | ImGuiInputTextFlags_WordWrap);
}

void page_sdl_audio_content(core::sdl_event_ctx &ctx){

	ImGui::BulletText("audio");
	if(ImGui::BeginChild("audio class",ImVec2{0,0},
		ImGuiChildFlags_Borders
		| ImGuiChildFlags_AutoResizeY 
		| ImGuiChildFlags_AlwaysAutoResize)){

		float gain=1.f;
		size_t sample_rate = 44100;
		size_t channels = 2;
		static float Hz = 440.f;//A4
		static std::vector<float> buf(sample_rate);
		static std::vector<Uint8> buf_wav;

		ImVec2 avail;
		static int buf_offset = 0;
		static uint64_t buf_wav_offset = 0;

		static bool once = true;
		if(once){
			//sin wave
			core::write_buf_sin_wave<float>(buf.data(),buf.size(),channels,sample_rate,Hz);

			//load wav
			auto &wav = romfs::get("wav/audio.wav");
			if(wav.data()){


				if(core::load_wav_from_mem_and_convert(
							wav.data(), wav.size(),
							ctx.callback_on_time_audio.get_src_spec(),
							buf_wav)){
					ctx.callback_on_time_audio.put_buf_data(buf_wav.data(), buf_wav.size());
				}

			}

			//bind
			ctx.realtime_audio.bind();
			ctx.callback_on_time_audio.bind();
			ctx.recording_audio.bind();

			//done
			once = false;
		}

		ImGui::SeparatorText("buf_audio freq");
		ImGui::Text("bufsize: %ld",sizeof(float) * buf.size());
		ImGui::Text("buf_wavsize: %ld", buf_wav.size());
		if(buf_wav.size() == 0){
			ImGui::Text("%s",SDL_GetError());
		}

		if(ImGui::DragFloat("Hz", &Hz)){
			core::write_buf_sin_wave<float>(buf.data(),buf.size(),channels,sample_rate,Hz);
		}

		#if 1
		avail = ImGui::GetContentRegionAvail();

		ImGui::SeparatorText("show wav");
		buf_wav_offset = ctx.callback_on_time_audio.get_read_pos();

		ImGui::SliderInt("buf offset",&buf_offset,0,buf.size()?buf.size()-1:0);
		ImGui::PlotHistogram("buf",buf.data(),buf.size(),
			buf_offset,nullptr,-1,1,ImVec2{avail.x*0.9f,avail.y*0.05f});

		ImGui::Separator();


		size_t callback_buf_start = 0;
		size_t callback_buf_size = ctx.callback_on_time_audio.get_buf().size();

		ImGui::ProgressBar(callback_buf_size?(float)buf_wav_offset/(float)callback_buf_size:0.f);

		if(ImGui::SliderScalar("read_pos",ImGuiDataType_U64,
					&buf_wav_offset,&callback_buf_start,&callback_buf_size)){
			ctx.callback_on_time_audio.set_read_pos(buf_wav_offset);
		}

		ImGui::PlotHistogram("callback",
			(const float*)ctx.callback_on_time_audio.get_buf().data(),
			ctx.callback_on_time_audio.get_buf().size()/sizeof(float),
			buf_wav_offset,nullptr,-1,1,ImVec2{avail.x*0.9f,avail.y*0.05f});
		#endif

		if(ImGui::BeginChild("audio class",ImVec2{0,0},
					ImGuiChildFlags_Borders
					| ImGuiChildFlags_AutoResizeY 
					| ImGuiChildFlags_AlwaysAutoResize)){
			#if 1
			ImGui::SeparatorText("realtime_audio vars");
			ImGui::Text("audio cur_did name: %s",ctx.realtime_audio.audio_device_name());
			ImGui::Text("audio status: %d",ctx.realtime_audio.is_ok());
			if(!ctx.realtime_audio.is_ok()){
				ImGui::Text("audio reason: %s",ctx.realtime_audio.what());
			}
			#endif

			#if 1
			ImGui::SeparatorText("realtime_audio control");
			ImGui::PushID("realtime_audio control");
			if (ImGui::Button("add voice")){
				ctx.realtime_audio.put_audio_stream_data(buf.data(), sizeof(float)*buf.size());
			}

			if (ImGui::Button("resume dev")){ctx.realtime_audio.resume_dev();}
			ImGui::SameLine();
			if (ImGui::Button("pause dev")){ctx.realtime_audio.pause_dev();}

			#if 0
			if (ImGui::Button("resume stream")){ctx.realtime_audio.resume();}
			ImGui::SameLine();
			if (ImGui::Button("pause stream")){ctx.realtime_audio.pause();}
			#endif

			if (ImGui::Button("clear")){ctx.realtime_audio.clear();}
			ImGui::SameLine();
			if (ImGui::Button("reset")){ctx.realtime_audio.reset_status();}

			gain = ctx.realtime_audio.get_volumn();
			if(ImGui::DragFloat("volumn gain",&gain,0.01f,0.f,10.f)){ctx.realtime_audio.volumn(gain);}
			ImGui::ProgressBar(gain/10.f);
			ImGui::PopID();
			#endif
		}
		ImGui::EndChild();

		if(ImGui::BeginChild("callback_on_time_audio class",ImVec2{0,0},
					ImGuiChildFlags_Borders
					| ImGuiChildFlags_AutoResizeY 
					| ImGuiChildFlags_AlwaysAutoResize)){
			#if 1
			ImGui::SeparatorText("callback_audio vars");
			ImGui::Text("audio cur_did name: %s",ctx.callback_on_time_audio.audio_device_name());
			ImGui::Text("audio is_paused: %d",ctx.callback_on_time_audio.is_paused());
			ImGui::Text("audio status: %d",ctx.callback_on_time_audio.is_ok());
			if(!ctx.callback_on_time_audio.is_ok()){
				ImGui::Text("audio reason: %s",ctx.callback_on_time_audio.what());
			}
			#endif

			#if 1
			ImGui::SeparatorText("callback_audio control");
			ImGui::PushID("callback_audio control");

			if (ImGui::Button("use buf voice")){
				ctx.callback_on_time_audio.put_buf_data(buf.data(), sizeof(float) * buf.size());
			}
			ImGui::SameLine();
			if (ImGui::Button("use audio voice")){
				ctx.callback_on_time_audio.put_buf_data(buf_wav.data(), buf_wav.size());
			}

			if (ImGui::Button("resume dev")){ctx.callback_on_time_audio.resume_dev();}
			ImGui::SameLine();
			if (ImGui::Button("pause dev")){ctx.callback_on_time_audio.pause_dev();}

			#if 0
			if (ImGui::Button("resume stream")){ctx.callback_on_time_audio.resume();}
			ImGui::SameLine();
			if (ImGui::Button("pause stream")){ctx.callback_on_time_audio.pause();}
			#endif

			if (ImGui::Button("clear")){ctx.callback_on_time_audio.clear();}
			ImGui::SameLine();
			if (ImGui::Button("reset")){ctx.callback_on_time_audio.reset_status();}

			gain = ctx.callback_on_time_audio.get_volumn();
			if(ImGui::DragFloat("volumn gain",&gain,0.01f,0.f,10.f)){
				ctx.callback_on_time_audio.volumn(gain);
			}
			ImGui::ProgressBar(gain/10.f);
			ImGui::PopID();
			#endif
		}
		ImGui::EndChild();

		if(ImGui::BeginChild("recording_audio class",ImVec2{0,0},
					ImGuiChildFlags_Borders
					| ImGuiChildFlags_AutoResizeY 
					| ImGuiChildFlags_AlwaysAutoResize)){
			#if 1
			ImGui::SeparatorText("recording_audio vars");
			ImGui::Text("audio cur_did name: %s",ctx.recording_audio.audio_device_name());
			ImGui::Text("audio is_paused: %d",ctx.recording_audio.is_paused());
			ImGui::Text("audio status: %d",ctx.recording_audio.is_ok());
			ImGui::Text("audio bufsize: %ld",ctx.recording_audio.get_buf().size());
			if(!ctx.recording_audio.is_ok()){
				ImGui::Text("audio reason: %s",ctx.recording_audio.what());
			}
			#endif

			#if 1
			ImGui::SeparatorText("recoding_audio control");
			ImGui::PushID("recoding_audio control");

			if (ImGui::Button("dump voice")){
				ctx.recording_audio.dump_audio_stream_data();

			}

			ImGui::SameLine();

			if (ImGui::Button("play voice")){
				size_t frame_size = sizeof(float) * channels;
				ctx.realtime_audio.put_audio_stream_data(
						ctx.recording_audio.get_buf().data(),
						static_cast<size_t>(ctx.recording_audio.get_buf().size()/frame_size) * frame_size);
			}

			if (ImGui::Button("resume dev")){ctx.recording_audio.resume_dev();}
			ImGui::SameLine();
			if (ImGui::Button("pause dev")){ctx.recording_audio.pause_dev();}

			#if 0
			if (ImGui::Button("resume stream")){ctx.recording_audio.resume();}
			ImGui::SameLine();
			if (ImGui::Button("pause stream")){ctx.recording_audio.pause();}
			#endif

			if (ImGui::Button("clear")){ctx.recording_audio.clear();}
			ImGui::SameLine();
			if (ImGui::Button("reset")){ctx.recording_audio.reset_status();}

			if (ImGui::Button("clear buf")){ctx.recording_audio.clear_buf();}

			gain = ctx.recording_audio.get_volumn();
			if(ImGui::DragFloat("volumn gain",&gain,0.01f,0.f,10.f)){ctx.recording_audio.volumn(gain);}
			ImGui::ProgressBar(gain/10.f);
			ImGui::PopID();
			#endif
		}
		ImGui::EndChild();

		ImGui::SeparatorText("audio log");

		core::log_sdl_audio_drivers(ImGui::TextWrapped);
		ImGui::Separator();
		core::log_sdl_audio_playback_devices(ImGui::TextWrapped);
		ImGui::Separator();
		core::log_sdl_audio_recording_devices(ImGui::TextWrapped);
	}
	ImGui::EndChild();

}

void page_sdl_camera_content(core::sdl_event_ctx &ctx){
	ImGui::BulletText("capture");
	if(ImGui::BeginChild("capture window",ImVec2{0,0},
		ImGuiChildFlags_Borders
		| ImGuiChildFlags_AutoResizeY
		| ImGuiChildFlags_AlwaysAutoResize)){

		ImGui::SeparatorText("vars");
		ImGui::TextWrapped("capture is_ok: %d",ctx.capture_device.is_ok());
		if(!ctx.capture_device.is_ok()){
			ImGui::TextWrapped("reason: %s",ctx.capture_device.what());
		}
		ImGui::Separator();
		//core::log_sdl_camera_spec(ctx.capture_device.get_current_capture_id());
		ImGui::TextWrapped("capture_device_num: %d",ctx.capture_device.get_capture_num());

		static Uint64 prev_timestamp_ns{};
		if(ctx.capture_device.get_timestamp_ns()){
			prev_timestamp_ns = ctx.capture_device.get_timestamp_ns();
		}

		ImGui::TextWrapped("prev timestamp: %ld ns",prev_timestamp_ns);
		ImGui::TextWrapped("capture pos: %s",core::get_name_sdl_camera_position(
			ctx.capture_device.get_pos()));
		ImGui::TextWrapped("capture permission state: %s",core::get_name_sdl_camera_permission_state(
			ctx.capture_device.get_permission()));

		if(SDL_CameraSpec spec; ctx.capture_device.get_fmt(&spec)){
			core::log_sdl_camera_spec(spec,ImGui::TextWrapped);
		}

		ImGui::SeparatorText("play control");

		static std::vector<const char*> items;
		static int current_item = 0;

		static std::string cur_spec;

		if(ImGui::Button("detect")){
			ctx.capture_device.detect_capture();
			items.resize(ctx.capture_device.get_capture_num());

			//map index to items index
			for(int i=0;i<items.size();i++){
				items[i] = SDL_GetCameraName(ctx.capture_device.get_capture_id(i));
			}
		}
		ImGui::SameLine();
		(void)ImGui::Combo("select", &current_item, items.data(), items.size());
		ImGui::SameLine();
		if(std::string spec_label; ImGui::BeginCombo("specs",cur_spec.c_str())){

			int specs_num{};
			SDL_CameraSpec **specs = SDL_GetCameraSupportedFormats(
				ctx.capture_device.get_current_capture_id(),
				&specs_num
			);

			for(int i=0;i<specs_num;i++){
				spec_label = std::format("{:d}x{:d} {:d}fps",
					(*specs[i]).width,(*specs[i]).height,
					(*specs[i]).framerate_denominator
						?(*specs[i]).framerate_numerator/(*specs[i]).framerate_denominator
						:0
					);

				ImGui::PushID(i);
				if(ImGui::Selectable(spec_label.c_str())){
					ctx.capture_device.open_capture(
						ctx.capture_device.get_current_capture_id(),
						specs[i]
					);

					cur_spec = spec_label;
				}
				ImGui::PopID();
			}

			SDL_free(specs);

			ImGui::EndCombo();
		}

		if(ImGui::Button("close")){
			ctx.capture_device.close_capture();
		}
		ImGui::SameLine();
		if (ImGui::Button("open")) {
			ctx.capture_device.open_capture(current_item);
		}

		ctx.capture_device.acquire_frame();
		static float h{},w{};

		SDL_Surface *frame = ctx.capture_device.get_frame();
		static float fh{},fw{};

		if(frame){
			h = float((*frame).h);
			w = float((*frame).w);

			SDL_Surface *cvt_frame = nullptr;
			if ((*frame).format != SDL_PIXELFORMAT_RGBA32){
				cvt_frame = SDL_ConvertSurface(frame,SDL_PIXELFORMAT_RGBA32);
			}else{
				cvt_frame = frame;
			}

			if(fw!=w || fh!=h){
				fw=w;fh=h;
				ctx.frame_tex.image2d(0,GL_RGBA,fw,fh,0,
						GL_RGBA,GL_UNSIGNED_BYTE,(*cvt_frame).pixels);
			}else{
				ctx.frame_tex.sub_image2d(0,0,0,fw,fh,
						GL_RGBA,GL_UNSIGNED_BYTE,(*cvt_frame).pixels);
			}

			if(cvt_frame!=frame)SDL_DestroySurface(cvt_frame);

			ctx.capture_device.release_frame();
		}

		ImVec2 avail = ImGui::GetContentRegionAvail();
		ImGui::Image(ctx.frame_tex.id(),ImVec2{avail.x,(fw==0.f?0.f:fh/fw)*avail.x});
		if(ImGui::Button("clear frame")){
			fw=fh=0.f;
			ctx.frame_tex.image2d(0,GL_RGBA,fw,fh,0,GL_RGBA,GL_UNSIGNED_BYTE,nullptr);
		}

		ImGui::SeparatorText("log");
		core::log_sdl_camera_drivers(ImGui::TextWrapped);
		ImGui::Separator();
		core::log_sdl_camera(ImGui::TextWrapped);
	}

	ImGui::EndChild();
}

void page_sdl_event_content(core::sdl_event_ctx &ctx){

	static bool auto_scroll = true;
	ImGui::Checkbox("Auto-scroll", &auto_scroll);
	ImGui::SameLine();
	if (ImGui::Button("Clear")) { ctx.e_tbuf.txtbuf.clear(); }
	ImGui::SameLine();
	if (ImGui::Button("Copy")) {
		ImGui::SetClipboardText(ctx.e_tbuf.txtbuf.c_str());
	}
	ImGui::Separator();

	if(ImGui::BeginChild("log_scroll", ImVec2(0, 0),
		ImGuiChildFlags_None,
		ImGuiWindowFlags_HorizontalScrollbar)){

		ImGui::TextWrapped("%s", ctx.e_tbuf.txtbuf.c_str());

		if (auto_scroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()){
			ImGui::SetScrollHereY(1.0f);
		}
	}
	ImGui::EndChild();
}


void page_sdl_hints_content(core::sdl_event_ctx &ctx){
	core::log_sdl_hints(ImGui::TextWrapped);
}

void page_sdl_mics_content(core::sdl_event_ctx &ctx){

	ImGui::SeparatorText("sdl init flags status");
	core::log_sdl_init_flags(ImGui::TextWrapped);

	ImGui::SeparatorText("main thread");
	core::log_sdl_is_main_thread(ImGui::TextWrapped);

	ImGui::SeparatorText("metadata");
	core::log_sdl_app_metadata_properties(ImGui::TextWrapped);

	ImGui::SeparatorText("keyboard");
	ctx.keyboard.log(ImGui::TextWrapped);

	ImGui::SeparatorText("mouse");
	core::log_sdl_mouse(ImGui::TextWrapped);
	core::log_sdl_mouse_state(ImGui::TextWrapped);

	ImGui::SeparatorText("power state");
	core::log_sdl_power_info(ImGui::TextWrapped);

	ImGui::SeparatorText("render drivers");
	core::log_sdl_render_drivers(ImGui::TextWrapped);

	ImGui::SeparatorText("time");
	core::log_sdl_date_time_locale_preferences(ImGui::TextWrapped);
	//core::log_sdl_time(ImGui::TextWrapped);
	core::log_chrono_time(ImGui::TextWrapped);

	ImGui::SeparatorText("version");
	core::log_sdl_version(ImGui::TextWrapped);

	ImGui::SeparatorText("drivers");
	core::log_sdl_num_video_drivers(ImGui::TextWrapped);
	core::log_sdl_video_drivers(ImGui::TextWrapped);

	ImGui::SeparatorText("theme");
	core::log_sdl_system_theme(ImGui::TextWrapped);

	ImGui::SeparatorText("primary_display");
	core::log_sdl_primary_display(ImGui::TextWrapped);

	ImGui::SeparatorText("displays");
	core::log_sdl_displays(ImGui::TextWrapped);

	ImGui::SeparatorText("windows");
	core::log_sdl_windows(ImGui::TextWrapped);
}


void page_sdl_hit_test_content(core::sdl_event_ctx &ctx){

	static bool start = false;
	static bool prepare_start = true;
	static int64_t duration_ms = 1000, prepare_time=1000;
	static size_t clicks = 0,clicks_record = 0;
	static std::chrono::steady_clock::time_point time_to_end;
	static std::chrono::steady_clock::rep time_left = 0;

	static bool enable_hit_test = true;
	ImGui::Toggle("enable_hit_test(turn off for adjusting)",&enable_hit_test);
	if(enable_hit_test){
		//ctx.keyboard.state is cur_key_frame
		static std::vector<bool> prev_key_frame(ctx.keyboard.numkeys);

		if (start){

			for(int i=0; i<prev_key_frame.size(); i++){
				//prev[i]==1 and cur[i] ==0 mean key up
				if(prev_key_frame[i] && !ctx.keyboard.state[i]){
					clicks++;
				}
			}

			time_left = std::chrono::duration_cast<std::chrono::milliseconds>(
					time_to_end-std::chrono::steady_clock::now())
				.count();

			if(time_left <= 0){
				time_left = 0;
				start = false;
			}else{
				prev_key_frame.assign(ctx.keyboard.state,ctx.keyboard.state+ctx.keyboard.numkeys);
			}

		}else if(prepare_start){
			clicks_record = clicks;

			//press any key to start
			for(int i=0;i<ctx.keyboard.numkeys;i++){
				if(ctx.keyboard.state[i]){
					time_to_end = std::chrono::steady_clock::now()
						+ std::chrono::milliseconds(duration_ms);

					clicks = 0;
					start = true;
					prepare_start = false;
				}
			}

			prev_key_frame.assign(ctx.keyboard.state,ctx.keyboard.state+ctx.keyboard.numkeys);

		}else{
			if(time_to_end + std::chrono::milliseconds(prepare_time)
					< std::chrono::steady_clock::now()){
				prepare_start = true;
			}
		}

		ImGui::TextColored(ImVec4(0.8,0,0.5,1),"clicks_record: %ld",clicks_record);
		ImGui::TextColored(prepare_start?ImVec4(0,0.8,0.6,1):ImVec4(0.8,0,0,1),
				"prepare_start: %s",prepare_start?"ready":"waiting");

		ImGui::Text("prepare_time_ms: %ld",prepare_time);
		ImGui::SameLine();
		ImGui::Text("duration_ms: %ld",duration_ms);

		DrawCountCard(clicks);
		ImGui::SameLine();
		DrawCountCard(time_left,"Time left","ms","current");

	}else{
		time_left = 0;
		start = false;
		time_to_end = std::chrono::steady_clock::now();

		ImGui::TextColored(ImVec4(0.8,0,0.5,1),"clicks_record: %ld",clicks_record);
		ImGui::TextColored(prepare_start?ImVec4(0,0.8,0.6,1):ImVec4(0.8,0,0,1),
				"prepare_start: %s",prepare_start?"ready":"waiting");

		ImGui::DragScalar("prepare_time_ms",ImGuiDataType_S64,&prepare_time);
		ImGui::DragScalar("duration_ms",ImGuiDataType_S64,&duration_ms);

		DrawCountCard(clicks);
		ImGui::SameLine();
		DrawCountCard(time_left,"Time left","ms","current");

	}

}

void page_cmd_content(core::sdl_event_ctx &ctx){
	//time control
	std::chrono::steady_clock::time_point tp_now = std::chrono::steady_clock::now();
	static std::chrono::steady_clock::time_point tp_last{};
	static int delay = 1000;

	//status control
	static bool turn_on_off = false;

	ImGui::SeparatorText("run command");

	//adjust delay values
	ImGui::DragInt("delay_ms",&delay);
	if(delay < 0)delay = 0;

	//cmd text
	static std::string command;
	if(command.capacity() < 1024){
		command.reserve(1024);
	}

	//clipboard textline
	static std::string clipboard_textline;
	if(clipboard_textline.capacity() < 1024){
		clipboard_textline.reserve(1024);
	}

	//wheather or not to update clipboard to next line
	static bool update_clipboard = false;
	static size_t clipboard_line_index=0;

	//resize for input multiline
	auto resize_callback = [](ImGuiInputTextCallbackData* data) -> int {
		if(!data) return 0;

		if ((*data).EventFlag == ImGuiInputTextFlags_CallbackResize) {
			auto* str = (std::string*)(*data).UserData;
			(*str).resize((*data).BufSize);
			(*data).Buf = (*str).data();
		}


		return 0;
	};

	//avail
	ImVec2 avail = ImGui::GetContentRegionAvail();

	//preset command
	static int cur_item = 0;
	static std::vector<std::string> preset_commands = {
		"xdotool key --window id ctrl+v enter",
		"xdotool key --window id ctrl+v shift+enter",
		"xdotool key --window id ctrl+shift+v enter",

		"ydotool key 29:1 47:1 47:0 29:0 28:1 28:0 #ctrl+v enter",
		"ydotool key 29:1 47:1 47:0 29:0 42:1 28:1 28:0 42:0 #ctrl+v shift+enter",
		"ydotool key 29:1 42:1 47:1 47:0 42:0 29:0 28:1 28:0 #ctrl+shift+v enter",

		"xdotool type --window id adadadadad",
		"xdotool type --window id wswswswsws",
	};

	if(ImGui::BeginCombo("select", command.c_str())){
		for(size_t i=0;i<preset_commands.size();i++){
			ImGui::PushID(i);
			if(ImGui::Selectable(preset_commands[i].c_str())){
				command = preset_commands[i];
			}
			ImGui::PopID();
		}

		ImGui::EndCombo();
	}


	//input command
	ImGui::InputTextMultiline("##command",
			command.data(), command.capacity(),
			ImVec2(avail.x,0.4f * avail.y),
			ImGuiInputTextFlags_CallbackResize,
			resize_callback,
			&command);

	//update command
	if(!turn_on_off){//write lock
		std::lock_guard<std::mutex> write_lock(ctx.cworker_ctl.status.mtx);
		ctx.cworker_ctl.cmd.assign(command.data(), command.size());
	}

	//run command
	if (turn_on_off && tp_last + std::chrono::milliseconds(delay) < tp_now){
		tp_last = tp_now;

		//update_clipboard
		if(update_clipboard){
			clipboard_line_index++;
			if(clipboard_line_index > std::count(clipboard_textline.begin(),clipboard_textline.end(),'\n')){
				clipboard_line_index = 0;
			}

			size_t l=0,r=0;
			for(size_t line = 0;l < clipboard_textline.size();l++){
				if(line == clipboard_line_index)break;
				else if(clipboard_textline[l] == '\n')line++;
			}

			for(r=l; r+1 < clipboard_textline.size();){
				r++;
				if(clipboard_textline[r] == '\n')break;
			}

			ImGui::SetClipboardText(clipboard_textline.substr(l,r-l).c_str());

			#if 0
			std::istringstream ss(clipboard_textline);
			std::string textline;
			for (int i = 0; i <= clipboard_line_index && std::getline(ss, textline); ++i) {
				if (i == clipboard_line_index){
					ImGui::SetClipboardText(textline.c_str());
				}
			}
			#endif
		}

		//try to weak up
		ctx.cworker_ctl.try_to_wake_up_worker();
		std::this_thread::yield();
	}

	//turn on/off
	(void)ImGui::Toggle("turn on/off",&turn_on_off);

	//return value
	ImGui::Text("system_ret: %d",ctx.cworker_ctl.system_ret);


	//input textline for clipboard
	ImGui::SeparatorText("update clipboard to next textline");

	if(ImGui::Button("clear clipboard textline")){
		clipboard_textline.clear();
	}

	ImGui::SameLine();
	(void)ImGui::Toggle("update clipboard", &update_clipboard);

	ImGui::InputTextMultiline("##clipboard_textline",
			clipboard_textline.data(), clipboard_textline.capacity(),
			ImVec2(avail.x,0.4f * avail.y),
			ImGuiInputTextFlags_CallbackResize,
			resize_callback,
			&clipboard_textline);

}

void DrawCountCard(
		int64_t count,
		std::string title,
		std::string num_suffix,
		std::string label,
		ImVec2 size,
		float rounding,
		float title_font_size,
		float num_font_size,
		float label_font_size
		){


	auto anime_color = [](ImU32 startColor, float speed = 0.08f) -> ImU32 {
		ImVec4 rgba = ImGui::ColorConvertU32ToFloat4(startColor);

		float h{}, s{}, v{};
		ImGui::ColorConvertRGBtoHSV(rgba.x, rgba.y, rgba.z, h, s, v);

		float time = ImGui::GetTime() * speed;
		h = fmodf(h + time, 1.0f);

		ImVec4 ret_rgba;
		ImGui::ColorConvertHSVtoRGB(h, s, v, ret_rgba.x, ret_rgba.y, ret_rgba.z);
		ret_rgba.w = rgba.w;

		return ImGui::ColorConvertFloat4ToU32(ret_rgba);
	};

	ImDrawList* draw = ImGui::GetWindowDrawList();
	ImVec2 card_pos = ImGui::GetCursorScreenPos();
	//ImVec2 card_size = ImGui::GetContentRegionAvail();
	ImVec2 card_size = size;

	ImU32 col_bg = anime_color(IM_COL32(0x39, 0xc5, 0xbb, 0xff));
	ImU32 col_tx = anime_color(IM_COL32(0xf5, 0xf5, 0xf5, 0xf5));

	//bg and shadow and border
	ImVec2 bg_pos_max = ImVec2(card_pos.x + card_size.x, card_pos.y + card_size.y);
	draw->AddRectFilled(card_pos, bg_pos_max , col_bg, rounding);
	draw->AddRect(card_pos, bg_pos_max , col_tx, rounding);
	draw->AddRectFilled(card_pos, bg_pos_max , IM_COL32(0, 0, 0, 0x32), rounding);

	ImVec2 title_pos = ImVec2(card_pos.x + 0.1*card_size.x, card_pos.y + 0.1*card_size.y);
	(*draw).AddText(ImGui::GetFont(), title_font_size, title_pos, col_tx, title.c_str());

	std::string buf = std::to_string(count);
	buf += num_suffix;
	ImVec2 num_pos = ImVec2(card_pos.x  + 0.2*card_size.x, card_pos.y + 0.5*card_size.y);
	(*draw).AddText(ImGui::GetFont(), num_font_size, num_pos, col_tx , buf.c_str());

	ImVec2 line_pos_min = ImVec2(card_pos.x  + 0.2*card_size.x, card_pos.y + 0.8*card_size.y);
	ImVec2 line_pos_max = ImVec2(line_pos_min.x + 0.7 *card_size.x,line_pos_min.y + 0.01*card_size.y);
	(*draw).AddRect(line_pos_min, line_pos_max, col_tx, rounding);

	ImVec2 label_pos = ImVec2(card_pos.x  + 0.25*card_size.x, card_pos.y + 0.86*card_size.y);
	(*draw).AddText(ImGui::GetFont(), label_font_size, label_pos, col_tx, label.c_str());

	ImGui::Dummy(size);
}

void ColorfulStyle(){
	ImGuiStyle& style = ImGui::GetStyle();

	// ---------- Font scaling ----------
	style.FontSizeBase = 18.0000f;
	style.FontScaleMain = 1.0000f;
	style.FontScaleDpi = 1.0000f;

	// ---------- Alpha ----------
	style.Alpha = 1.0000f;
	style.DisabledAlpha = 0.6000f;

	// ---------- Window ----------
	style.WindowPadding = ImVec2(8.0000f, 8.0000f);
	style.WindowRounding = 12.0000f;
	style.WindowBorderSize = 1.0000f;
	style.WindowBorderHoverPadding = 4.0000f;
	style.WindowMinSize = ImVec2(32.0000f, 32.0000f);
	style.WindowTitleAlign = ImVec2(0.0000f, 0.5000f);
	style.WindowMenuButtonPosition = (ImGuiDir)0;

	// ---------- Child & Popup ----------
	style.ChildRounding = 12.0000f;
	style.ChildBorderSize = 1.0000f;
	style.PopupRounding = 12.0000f;
	style.PopupBorderSize = 1.0000f;

	// ---------- Frame & Item ----------
	style.FramePadding = ImVec2(4.0000f, 3.0000f);
	style.FrameRounding = 12.0000f;
	style.FrameBorderSize = 0.0000f;
	style.ItemSpacing = ImVec2(8.0000f, 4.0000f);
	style.ItemInnerSpacing = ImVec2(4.0000f, 4.0000f);
	style.CellPadding = ImVec2(4.0000f, 2.0000f);
	style.TouchExtraPadding = ImVec2(0.0000f, 0.0000f);
	style.IndentSpacing = 21.0000f;
	style.ColumnsMinSpacing = 6.0000f;

	// ---------- Scrollbar ----------
	style.ScrollbarSize = 14.0000f;
	style.ScrollbarRounding = 12.0000f;
	style.ScrollbarPadding = 2.0000f;

	// ---------- Grab ----------
	style.GrabMinSize = 12.0000f;
	style.GrabRounding = 12.0000f;
	style.LogSliderDeadzone = 4.0000f;

	// ---------- Image ----------
	style.ImageRounding = 12.0000f;
	style.ImageBorderSize = 0.0000f;

	// ---------- Tab ----------
	style.TabRounding = 12.0000f;
	style.TabBorderSize = 0.0000f;
	style.TabMinWidthBase = 1.0000f;
	style.TabMinWidthShrink = 80.0000f;
	style.TabCloseButtonMinWidthSelected = -1.0000f;
	style.TabCloseButtonMinWidthUnselected = 0.0000f;
	style.TabBarBorderSize = 1.0000f;
	style.TabBarOverlineSize = 1.0000f;

	// ---------- Table ----------
	style.TableAngledHeadersAngle = 0.6109f;
	style.TableAngledHeadersTextAlign = ImVec2(0.5000f, 0.0000f);

	// ---------- TreeLines ----------
	style.TreeLinesFlags = (ImGuiTreeNodeFlags)262144;
	style.TreeLinesSize = 1.0000f;
	style.TreeLinesRounding = 12.0000f;

	// ---------- MenuItem & Selectable ----------
	style.MenuItemRounding = 12.0000f;
	style.SelectableRounding = 12.0000f;
	style.DragDropTargetRounding = 12.0000f;
	style.DragDropTargetBorderSize = 2.0000f;
	style.DragDropTargetPadding = 3.0000f;
	style.ColorMarkerSize = 3.0000f;
	style.ColorButtonPosition = (ImGuiDir)1;
	style.ButtonTextAlign = ImVec2(0.5000f, 0.5000f);
	style.SelectableTextAlign = ImVec2(0.0000f, 0.0000f);
	style.InputTextCursorSize = 1.0000f;

	// ---------- Separator ----------
	style.SeparatorSize = 1.0000f;
	style.SeparatorTextBorderSize = 3.0000f;
	style.SeparatorTextAlign = ImVec2(0.0000f, 0.5000f);
	style.SeparatorTextPadding = ImVec2(20.0000f, 3.0000f);

	// ---------- Display ----------
	style.DisplayWindowPadding = ImVec2(19.0000f, 19.0000f);
	style.DisplaySafeAreaPadding = ImVec2(3.0000f, 3.0000f);

	// ---------- Docking ----------
	style.DockingNodeHasCloseButton = true;
	style.DockingSeparatorSize = 2.0000f;

	// ---------- Mouse Cursor ----------
	style.MouseCursorScale = 1.0000f;

	// ---------- Anti-Aliasing ----------
	style.AntiAliasedLines = true;
	style.AntiAliasedLinesUseTex = true;
	style.AntiAliasedFill = true;

	// ---------- Tessellation ----------
	style.CurveTessellationTol = 1.2500f;
	style.CircleTessellationMaxError = 0.3000f;

	// ---------- Colors ----------
	style.Colors[ImGuiCol_Text] = ImVec4(0.9000f, 0.9000f, 0.9000f, 1.0000f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.6000f, 0.6000f, 0.6000f, 1.0000f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.0000f, 0.0000f, 0.0000f, 0.8500f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.0000f, 0.0000f, 0.0000f, 0.0000f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.1100f, 0.1100f, 0.1400f, 0.9200f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.5000f, 0.5000f, 0.5000f, 0.5000f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0000f, 0.0000f, 0.0000f, 0.0000f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.4300f, 0.4300f, 0.4300f, 0.3900f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.4700f, 0.4700f, 0.6900f, 0.4000f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.4200f, 0.4100f, 0.6400f, 0.6900f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.2700f, 0.2700f, 0.5400f, 0.8300f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.3200f, 0.3200f, 0.6300f, 0.8700f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.4000f, 0.4000f, 0.8000f, 0.2000f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.4000f, 0.4000f, 0.5500f, 0.8000f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.2000f, 0.2500f, 0.3000f, 0.6000f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.4000f, 0.4000f, 0.8000f, 0.3000f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.4000f, 0.4000f, 0.8000f, 0.4000f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.4100f, 0.3900f, 0.8000f, 0.6000f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.9000f, 0.9000f, 0.9000f, 0.5000f);
	style.Colors[ImGuiCol_CheckboxSelectedBg] = ImVec4(0.4235f, 0.4170f, 0.5665f, 0.5850f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(1.0000f, 1.0000f, 1.0000f, 0.3000f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.4100f, 0.3900f, 0.8000f, 0.6000f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.3500f, 0.4000f, 0.6100f, 0.6200f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.4000f, 0.4800f, 0.7100f, 0.7900f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.4600f, 0.5400f, 0.8000f, 1.0000f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.4000f, 0.4000f, 0.9000f, 0.4500f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.4500f, 0.4500f, 0.9000f, 0.8000f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.5300f, 0.5300f, 0.8700f, 0.8000f);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.5000f, 0.5000f, 0.5000f, 0.6000f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.6000f, 0.6000f, 0.7000f, 1.0000f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.7000f, 0.7000f, 0.9000f, 1.0000f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.0000f, 1.0000f, 1.0000f, 0.1000f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.7800f, 0.8200f, 1.0000f, 0.6000f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.7800f, 0.8200f, 1.0000f, 0.9000f);
	style.Colors[ImGuiCol_InputTextCursor] = ImVec4(0.9000f, 0.9000f, 0.9000f, 1.0000f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.4500f, 0.4500f, 0.9000f, 0.8000f);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.3360f, 0.3360f, 0.6840f, 0.7860f);
	style.Colors[ImGuiCol_TabSelected] = ImVec4(0.4040f, 0.4040f, 0.7260f, 0.8420f);
	style.Colors[ImGuiCol_TabSelectedOverline] = ImVec4(0.5300f, 0.5300f, 0.8700f, 0.8000f);
	style.Colors[ImGuiCol_TabDimmed] = ImVec4(0.2832f, 0.2832f, 0.5688f, 0.8212f);
	style.Colors[ImGuiCol_TabDimmedSelected] = ImVec4(0.3504f, 0.3504f, 0.6516f, 0.8372f);
	style.Colors[ImGuiCol_TabDimmedSelectedOverline] = ImVec4(0.5300f, 0.5300f, 0.8700f, 0.0000f);
	style.Colors[ImGuiCol_DockingPreview] = ImVec4(0.4000f, 0.4000f, 0.9000f, 0.3150f);
	style.Colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.2000f, 0.2000f, 0.2000f, 1.0000f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(1.0000f, 1.0000f, 1.0000f, 1.0000f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.9000f, 0.7000f, 0.0000f, 1.0000f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.9000f, 0.7000f, 0.0000f, 1.0000f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.0000f, 0.6000f, 0.0000f, 1.0000f);
	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.2700f, 0.2700f, 0.3800f, 1.0000f);
	style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.3100f, 0.3100f, 0.4500f, 1.0000f);
	style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.2600f, 0.2600f, 0.2800f, 1.0000f);
	style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0000f, 0.0000f, 0.0000f, 0.0000f);
	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0000f, 1.0000f, 1.0000f, 0.0700f);
	style.Colors[ImGuiCol_TextLink] = ImVec4(0.5300f, 0.5300f, 0.8700f, 0.8000f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.0000f, 0.0000f, 1.0000f, 0.3500f);
	style.Colors[ImGuiCol_TreeLines] = ImVec4(0.5000f, 0.5000f, 0.5000f, 0.5000f);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.0000f, 1.0000f, 0.0000f, 0.9000f);
	style.Colors[ImGuiCol_DragDropTargetBg] = ImVec4(0.0000f, 0.0000f, 0.0000f, 0.0000f);
	style.Colors[ImGuiCol_UnsavedMarker] = ImVec4(0.9000f, 0.9000f, 0.9000f, 1.0000f);
	style.Colors[ImGuiCol_NavCursor] = ImVec4(0.4500f, 0.4500f, 0.9000f, 0.8000f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0000f, 1.0000f, 1.0000f, 0.7000f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.8000f, 0.8000f, 0.8000f, 0.2000f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.2000f, 0.2000f, 0.2000f, 0.3500f);

	// ---------- Hover Behaviors ----------
	style.HoverStationaryDelay = 0.1500f;
	style.HoverDelayShort = 0.1500f;
	style.HoverDelayNormal = 0.4000f;
	style.HoverFlagsForTooltipMouse = (ImGuiHoveredFlags)41984;
	style.HoverFlagsForTooltipNav = (ImGuiHoveredFlags)197632;
}

