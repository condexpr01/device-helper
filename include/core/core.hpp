#ifndef CORE_HEADER_GUARD
#define CORE_HEADER_GUARD

#include "glad/gl.hpp"

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl3.h"
//#include "implot.h"
//#include "implot3d.h"
//#include "imnodes.h"
#include "imgui_toggle.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

#include <condition_variable>
#include <filesystem>

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <mutex>
#include <thread>

#include <vector>

#include "core-types.hpp"
#include "gl-camera.hpp"
#include "gl-program.hpp"
#include "gl-texture.hpp"
#include "gl-shader.hpp"
#include "gl-vertex-array.hpp"
#include "gl-framebuffer.hpp"
#include "gl-renderbuffer.hpp"

#include "sdl-keyboard.hpp"
#include "sdl-audio.hpp"
#include "sdl-camera.hpp"
#include "sdl-event.hpp"
#include "sdl-hints.hpp"
#include "sdl-init.hpp"
#include "sdl-time.hpp"
#include "sdl-video.hpp"

#include "romfs/romfs.hpp"

namespace core{

	//manage sdl init/quit context
	class sdl_ctx_manager{
		//error status
		private:
			bool status = false;
			const char* reason = nullptr;

		//get error status methods
		public:
			bool       is_ok() noexcept{return status;}
			const char* what() noexcept{return reason;}

		//RAII
		public:
			sdl_ctx_manager() noexcept{
				if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)){
					status = false;
					reason = "[sdl_ctx_manager] SDL_Init";
					return;
				}

				status = true;
			}

			~sdl_ctx_manager() noexcept{SDL_Quit();}

			//neither movable nor copyable
			sdl_ctx_manager& operator=(sdl_ctx_manager  &s) =delete;
			sdl_ctx_manager& operator=(sdl_ctx_manager &&s) =delete;
	};

	//manage SDL_GLContext
	class sdl_gl_ctx_manager{
		//error status
		private:
			bool status = false;
			const char* reason = nullptr;

		//get error status methods
		public:
			bool       is_ok() noexcept{return status;}
			const char* what() noexcept{return reason;}

		//res
		public:
			SDL_GLContext gl_ctx = nullptr;

		//methods
		public:

			void create(SDL_Window *window) noexcept{
				if (gl_ctx){release();}

				if (window){
					gl_ctx = SDL_GL_CreateContext(window);
				}else{
					status = false;
					reason = "[sdl_gl_ctx_manager] invalid SDL_Window*";
					return;
				}

				if (!gl_ctx){
					status = false;
					reason = "[sdl_gl_ctx_manager] invalid SDL_GLContext";
					return;
				}

				if (!gladLoadGL(SDL_GL_GetProcAddress)){
					status = false;
					reason = "[sdl_gl_ctx_manager] gladLoadGL";
					return;
				}

				status = true;
			}

			void release() noexcept{if(gl_ctx){SDL_GL_DestroyContext(gl_ctx);}}


		//RAII
		public:
			sdl_gl_ctx_manager() noexcept = default;
			sdl_gl_ctx_manager(SDL_Window *window) noexcept{create(window);}

			//movable but not copyable
			sdl_gl_ctx_manager &operator=(sdl_gl_ctx_manager  &s) = delete;
			sdl_gl_ctx_manager &operator=(sdl_gl_ctx_manager &&s) = default;

			~sdl_gl_ctx_manager() noexcept{release();}
	};

	//manage sdl3 gl3 impl and imgui ctx
	class sdl3_gl3_imgui_ctx_manager{
		//error status
		private:
			bool status = false;
			const char* reason = nullptr;

		//get error status methods
		public:
			bool       is_ok() noexcept{return status;}
			const char* what() noexcept{return reason;}

		//res
		public:
			ImGuiContext *imgui_ctx = nullptr;
			//ImPlotContext *implot_ctx = nullptr;
			//ImPlot3DContext *implot3d_ctx = nullptr;
			//ImNodesContext *imnodes_ctx = nullptr;

			ImFont* font = nullptr;
			const float fontsize = 18.f;

		//RAII
		public:
			sdl3_gl3_imgui_ctx_manager(SDL_Window *glwindow,SDL_GLContext glctx) noexcept{
				imgui_ctx = ImGui::CreateContext();
				//implot_ctx = ImPlot::CreateContext();
				//implot3d_ctx = ImPlot3D::CreateContext();
				//imnodes_ctx = ImNodes::CreateContext();

				if (!imgui_ctx 
						//|| !implot_ctx 
						//|| !implot3d_ctx 
						//|| !imnodes_ctx
				   ){
					status = false;
					reason = "[sdl_gl_imgui_ctx_manager]CreateContext";
					return;
				}

				if(!ImGui_ImplSDL3_InitForOpenGL(glwindow,glctx)){
					status = false;
					reason = "[sdl_gl_imgui_ctx_manager]ImGui_ImplSDL3_InitForOpenGL";
					return;
				}

				if(!ImGui_ImplOpenGL3_Init("#version 460")){
					status = false;
					reason = "[sdl_gl_imgui_ctx_manager]ImGui_ImplOpenGL3_Init";
					return;
				}

				status = true;


				ImFontConfig f{};
				//f.Flags = ImFontFlags_NoLoadError;
				f.FontDataOwnedByAtlas = false;

				ImGuiIO &io = ImGui::GetIO();

				#if 1
				const romfs::Resource &ttf = romfs::get("fonts/SarasaUiSC-Bold.ttf");
				if(ttf.data()){
					font = (*io.Fonts).AddFontFromMemoryTTF(
						(void*)ttf.data(),ttf.size(),
						fontsize,&f,nullptr
					);

					io.FontDefault = font;
				}
				#else
				//default font
				std::filesystem::path font_path = std::filesystem::path{SDL_GetBasePath()}
					/ "fonts"/ "SarasaUiSC-Bold.ttf";

				if (std::filesystem::exists(font_path)){
					font = (*io.Fonts).AddFontFromFileTTF(font_path.c_str(),fontsize,&f,nullptr);
					io.FontDefault = font;
				}
				#endif

				io.IniFilename = nullptr;

				//when sudo, ImGuiConfigFlags_ViewportsEnable may cause crash, so disable it
				io.ConfigFlags &= ~ImGuiConfigFlags_ViewportsEnable;
			}

			~sdl3_gl3_imgui_ctx_manager() noexcept{
				ImGui_ImplSDL3_Shutdown();
				ImGui_ImplOpenGL3_Shutdown();
				ImGui::DestroyContext(imgui_ctx);
				//ImPlot::DestroyContext(implot_ctx);
				//ImPlot3D::DestroyContext(implot3d_ctx);
				//ImNodes::DestroyContext(imnodes_ctx);
			}

			//neither movable nor copyable
			sdl3_gl3_imgui_ctx_manager &operator=(sdl3_gl3_imgui_ctx_manager  &s) = delete;
			sdl3_gl3_imgui_ctx_manager &operator=(sdl3_gl3_imgui_ctx_manager &&s) = delete;
	};

	class sdl_window_manager{
		//error status
		private:
			bool status = false;
			const char* reason = nullptr;

		//get error status methods
		public:
			bool       is_ok() noexcept{return status;}
			const char* what() noexcept{return reason;}

		//res
		public:
			SDL_Window *window = nullptr;

		//methods
		public:
			bool show() noexcept{return SDL_ShowWindow(window);}
			bool hide() noexcept{return SDL_HideWindow(window);}
			SDL_WindowFlags flags() noexcept{return SDL_GetWindowFlags(window);}

			GLboolean attributes(){
				return SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,24)
					&& SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE,8)
					&& SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8)
					&& SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8)
					&& SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8)
					&& SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8)
					&& SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
			}

		//RAII
		public:
			sdl_window_manager(const char* title, int w, int h, SDL_WindowFlags flags) noexcept{

				if(!attributes()){
					status = false;
					reason = "[sdl_window_manager] SDL_GL_SetAttribute";
					return;
				}

				if (!window){window = SDL_CreateWindow(title,w,h,flags);}

				if (!window){
					status = false;
					reason = "[sdl_window_manager]SDL_CreateWindow";
					return;
				}else{
					status = true;
				}

			}

			sdl_window_manager() noexcept{

				if(!attributes()){
					status = false;
					reason = "[sdl_window_manager] SDL_GL_SetAttribute";
					return;
				}

				int denominator = 2;

				SDL_DisplayID did = SDL_GetPrimaryDisplay();
				const SDL_DisplayMode *dm = SDL_GetCurrentDisplayMode(did);
				if (!dm){
					status = false;
					reason = "[sdl_window_manager]SDL_GetCurrentDisplayMode";
					return;
				}

				if (!window){
					window = SDL_CreateWindow("sdl3-gl3-imgui",
							(*dm).w/denominator,(*dm).h/denominator,
							SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
				}

				if (!window){
					status = false;
					reason = "[sdl_window_manager]SDL_CreateWindow";
					return;
				}else{
					status = true;
				}
			}

			~sdl_window_manager() noexcept{
				if(window){SDL_DestroyWindow(window);}
			}
	};


	class sdl_tray_manager{
		//error status
		private:
			bool status = false;
			std::string reason;

		//get error status methods
		public:
			bool       is_ok() noexcept{return status;}
			const char* what() noexcept{return reason.c_str();}

		private:
			SDL_Surface *icon = nullptr;
			SDL_Tray *tray = nullptr;

			SDL_TrayMenu *tray_menu = nullptr;

			SDL_TrayEntry *tray_entry_sep = nullptr;
			SDL_TrayEntry *tray_entry_show_hide = nullptr;


		public:
			sdl_tray_manager(SDL_Window *window) noexcept{
				
				auto pngraw = romfs::get("icon/weapon.png");
				if(pngraw.data()){
					icon = SDL_LoadPNG_IO(SDL_IOFromMem((void*)pngraw.data(), pngraw.size()), true);
				}

				if(!icon){
					status = false;
					reason = "[sdl_tray_manager] icon nullptr";
					return;
				}

				tray = SDL_CreateTray(icon,"tray");
				if (!tray){status = false;reason = SDL_GetError();return;}

				tray_menu = SDL_CreateTrayMenu(tray);
				if (!tray_menu){status = false;reason = SDL_GetError();return;}

				tray_entry_sep = SDL_InsertTrayEntryAt(tray_menu,-1,nullptr,0);
				tray_entry_show_hide = SDL_InsertTrayEntryAt(tray_menu,-1,"show/hide",SDL_TRAYENTRY_CHECKBOX);

				SDL_SetTrayEntryCallback(tray_entry_show_hide,[](void *userdata, SDL_TrayEntry *entry)->void{
					if(!userdata)return;

					if(SDL_GetWindowFlags((SDL_Window*)userdata) & SDL_WINDOW_HIDDEN){
						SDL_ShowWindow((SDL_Window*)userdata);
					}else{
						SDL_HideWindow((SDL_Window*)userdata);
					}

				},window);

			}

			~sdl_tray_manager() noexcept{
				if(tray)SDL_DestroyTray(tray);
				if(icon)SDL_DestroySurface(icon);
			}
	};


	class cmd_worker{
		public:
			class worker_status{
				public:
					std::mutex mtx;
					std::condition_variable_any cv;

					bool has_jobs = false;
					bool done = false;

					auto get_status() noexcept{ return [this](){return has_jobs || done;};}
					void reset_status() noexcept{ has_jobs = false; }
			} status{};

			//void (*jobs)();//pimpl
			//
			std::string cmd{};
			int system_ret{};

			void jobs(){
				system_ret = std::system(cmd.c_str());
			}

		public:
			void worker_func(std::stop_token stoken){
				std::unique_lock<std::mutex> lock{status.mtx};

				while(true){
					status.cv.wait(lock,stoken,status.get_status());

					//quit
					if(stoken.stop_requested())break;
					if (status.done)break;

					jobs();

					status.reset_status();//for next waiting
				}
			}

			void force_to_wake_up_worker(){
				std::lock_guard<std::mutex> lock{status.mtx};

				status.has_jobs = true;
				status.cv.notify_all();
			}

			void try_to_wake_up_worker(){
				std::unique_lock<std::mutex> lock(status.mtx,std::try_to_lock_t{});

				if(lock.owns_lock()){
					status.has_jobs = true;
					status.cv.notify_one();
				}
			}

			//need to wakeup before thread join
			//manually call or use stop_token
			void release_worker(){
				std::lock_guard<std::mutex> lock{status.mtx};

				status.done = true;
				status.cv.notify_all();
			}

		public:
			~cmd_worker()=default;
	};

	enum page_status{
		PAGE_IO,
		PAGE_STYLE,
		PAGE_SDL_AUDIO,
		PAGE_SDL_CAMERA,
		PAGE_SDL_EVENT,
		PAGE_SDL_HINTS,
		PAGE_SDL_MICS,
		PAGE_HIT_TEST,
		PAGE_CMD,
	};

	struct sdl_event_ctx{
		//error status
		bool status = false;
		const char* reason = nullptr;

		//ctx vars
		SDL_Event e;
		bool running = false;

		//ctx managers
		sdl_window_manager &swm;
		sdl_gl_ctx_manager &sgcm;
		sdl3_gl3_imgui_ctx_manager &sgicm;

		//datas
		std::filesystem::path png_path;
		float leftw_pct = 0.2f;
		float bottomh_pct = 0.039f;

		page_status page = PAGE_CMD;
		bool window_draggable = false;

		//kbd
		core::keyboard keyboard{};

		//recording events
		class txtbuf{
			public:
				ImGuiTextBuffer txtbuf;
				void operator()(const char* fmt, ...){
					va_list args;
					va_start(args, fmt);
					txtbuf.appendfv(fmt, args);
					txtbuf.appendf("\n");
					va_end(args);
				}
		}e_tbuf;

		//cmd
		cmd_worker cworker_ctl{};
		std::jthread cworker{[this](std::stop_token stoken){
			(*this).cworker_ctl.worker_func(stoken);
		}};

		//audio
		core::realtime_audio realtime_audio{};
		core::callback_on_time_audio callback_on_time_audio{};
		core::recording_audio recording_audio{};

		//capture
		core::texture frame_tex{GL_TEXTURE_2D};
		core::capture_device capture_device{};

		//MVP
		//glm::mat4 model{1.f};
		//core::camera camera{};
		//glm::mat4 projection{1.f};

	};

	inline void set_up_sdl_event_ctx_datas(int argc, char **argv, sdl_event_ctx &ctx){

		#if 0
		if (argc < 2){
			throw std::runtime_error{
				"args: "
			};
		}
		#endif

	}

	class sdl_event_manager{
		//res
		public:
			sdl_event_ctx ctx;

		//get error status methods
		public:
			bool       is_ok() noexcept{return ctx.status;}
			const char* what() noexcept{return ctx.reason;}

		//methods
		public:
			sdl_event_ctx &get_ctx() noexcept{return ctx;}

			void loop();

		//RAII
		public:
			sdl_event_manager(
					sdl_window_manager &swm,
					sdl_gl_ctx_manager &sgcm,
					sdl3_gl3_imgui_ctx_manager &sgicm
					) noexcept
				: ctx{
					.swm = swm,
					.sgcm = sgcm,
					.sgicm = sgicm
				}
			{}

			~sdl_event_manager() noexcept{}

			//neither movable nor copyable
			sdl3_gl3_imgui_ctx_manager &operator=(sdl3_gl3_imgui_ctx_manager  &s) = delete;
			sdl3_gl3_imgui_ctx_manager &operator=(sdl3_gl3_imgui_ctx_manager &&s) = delete;
	};

}
#endif
