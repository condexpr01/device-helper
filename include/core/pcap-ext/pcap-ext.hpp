#ifndef CORE_HEADER_PCAP_EXT_GUARD
#define CORE_HEADER_PCAP_EXT_GUARD

#include <condition_variable>
#include <deque>
#include <mutex>
#include <pcap.h>
#include <string>
#include <unordered_map>
#include <vector>

#ifdef _WIN32
	typedef u_short sa_family_t;
#endif

namespace core{
	inline const char* get_name_pcap_af(sa_family_t family) noexcept{
		static std::unordered_map<sa_family_t, const char*> map = {
			{AF_UNSPEC,"AF_UNSPEC"},
			{AF_UNIX,"AF_UNIX"},
			{AF_INET,"AF_INET"},
			{AF_INET6,"AF_INET6"},
			{AF_IPX,"AF_IPX"},
			{AF_APPLETALK,"AF_APPLETALK"},
			{AF_DECnet,"AF_DECnet"},
			{AF_SNA,"AF_SNA"},
			{AF_IRDA,"AF_IRDA"},
			{AF_MAX,"AF_MAX"},
#ifdef __linux__
			{AF_LOCAL,"AF_LOCAL"},
			{AF_FILE,"AF_FILE"},
			{AF_AX25,"AF_AX25"},
			{AF_NETROM,"AF_NETROM"},
			{AF_BRIDGE,"AF_BRIDGE"},
			{AF_ATMPVC,"AF_ATMPVC"},
			{AF_X25,"AF_X25"},
			{AF_ROSE,"AF_ROSE"},
			{AF_NETBEUI,"AF_NETBEUI"},
			{AF_SECURITY,"AF_SECURITY"},
			{AF_KEY,"AF_KEY"},
			{AF_NETLINK,"AF_NETLINK"},
			{AF_ROUTE,"AF_ROUTE"},
			{AF_PACKET,"AF_PACKET"},
			{AF_ASH,"AF_ASH"},
			{AF_ECONET,"AF_ECONET"},
			{AF_ATMSVC,"AF_ATMSVC"},
			{AF_RDS,"AF_RDS"},
			{AF_PPPOX,"AF_PPPOX"},
			{AF_WANPIPE,"AF_WANPIPE"},
			{AF_LLC,"AF_LLC"},
			{AF_IB,"AF_IB"},
			{AF_MPLS,"AF_MPLS"},
			{AF_CAN,"AF_CAN"},
			{AF_TIPC,"AF_TIPC"},
			{AF_BLUETOOTH,"AF_BLUETOOTH"},
			{AF_IUCV,"AF_IUCV"},
			{AF_RXRPC,"AF_RXRPC"},
			{AF_ISDN,"AF_ISDN"},
			{AF_PHONET,"AF_PHONET"},
			{AF_IEEE802154,"AF_IEEE802154"},
			{AF_CAIF,"AF_CAIF"},
			{AF_ALG,"AF_ALG"},
			{AF_NFC,"AF_NFC"},
			{AF_VSOCK,"AF_VSOCK"},
			{AF_KCM,"AF_KCM"},
			{AF_QIPCRTR,"AF_QIPCRTR"},
			{AF_SMC,"AF_SMC"},
			{AF_XDP,"AF_XDP"},
			{AF_MCTP,"AF_MCTP"},
#endif
#ifdef _WIN32
			{AF_BTH,"AF_BTH"},
			{AF_NETBIOS,"AF_NETBIOS"},
			{AF_ATM,"AF_ATM"},
			{AF_PUP,"AF_PUP"},
#endif
		};

		auto find = map.find(family);
		if(find != map.end()){
			return (*find).second;
		}else{
			return "AF_UNKNOWN";
		}
	}

	class pcap_ext_init{
		//error status
		private:
			bool status = false;
			char reason[PCAP_ERRBUF_SIZE];

			//get error status methods
		public:
			bool       is_ok() noexcept{return status;}
			const char* what() noexcept{return reason;}

			//raii
			pcap_ext_init(unsigned int opt = PCAP_CHAR_ENC_UTF_8) noexcept{
				int ret = pcap_init(opt, reason);
				if(ret == 0){
					status = true;
				}else if(ret == PCAP_ERROR){
					status = false;
					return;
				}
			}

			~pcap_ext_init() = default;
	};


	class pcap_ext_alldevs{

		//error status
		private:
			bool status = false;
			char reason[PCAP_ERRBUF_SIZE];

			//get error status methods
		public:
			bool       is_ok() noexcept{return status;}
			const char* what() noexcept{return reason;}

			//inner vars
		private:
			pcap_if_t *alldevs = nullptr;

			//methods
		public:
			pcap_if_t *get_alldevs() noexcept{return alldevs;}

			void find() noexcept{
				if(alldevs){
					pcap_freealldevs(alldevs);
					alldevs = nullptr;
				}

				int ret = pcap_findalldevs(&alldevs, reason);
				if(ret == 0){
					status = true;
				}else if(ret == PCAP_ERROR){
					status = false;
				}
			}

			//raii
		public:
			pcap_ext_alldevs() noexcept{
				find();
			}

			~pcap_ext_alldevs() noexcept{
				if(alldevs){
					pcap_freealldevs(alldevs);
					alldevs = nullptr;
				}
			}

	};

	class pcap_ext_stats{

		//error status
		private:
			bool status = false;
			std::string reason;

			//get error status methods
		public:
			bool       is_ok() noexcept{return status;}
			const char* what() noexcept{return reason.c_str();}

		//inner vars
		private:
			struct pcap_stat stat;

		//methods
		public:

			//using handle update stat
			void update(pcap_t *handle) noexcept{
				int ret = pcap_stats(handle, &stat);
				if(ret == 0){
					status = true;
				}else if(ret == PCAP_ERROR || ret==PCAP_ERROR_NOT_ACTIVATED){
					status = false;
					reason = pcap_geterr(handle);
				}
			}

			//get stat
			struct pcap_stat &get_stat(){
				return stat;
			}

		//raii
		public:
			pcap_ext_stats() = default;
			~pcap_ext_stats() = default;

	};

	class pcap_ext_handle{

		//error status
		private:
			bool status = false;
			bool warning = false;
			std::string reason;

		//get error status methods
		public:
			bool       is_ok() noexcept{return status;}
			bool       is_warning() noexcept{return warning;}
			const char* what() noexcept{return reason.c_str();}

		//inner vars
		private:
			pcap_t *handle = nullptr;

		//methods
		public:
			pcap_t *get_handle() noexcept{return handle;}

			void create(const char *dev) noexcept{
				if(handle)pcap_close(handle);

				handle = pcap_create(dev, reason.data());
				if(handle){
					status = true;
					warning = false;
				}else{
					status = false;
					warning = false;
				}
			}

			void activate() noexcept{
				if(!status)return;
				int ret = pcap_activate(handle);
				if(ret == 0){
					status = true;
					warning = false;
				}else if(ret > 0){//warning
					status = true;
					warning = true;
					reason = pcap_geterr(handle);

				}else{//ret < 0, error
					status = false;
					warning = false;
					reason = pcap_geterr(handle);

					close();
				}
			}

			void close() noexcept{
				if(handle){
					pcap_close(handle);
					status = false;
					warning = false;
					handle = nullptr;
				}
			}

			//before activate
			void set_snaplen(int len) noexcept{
				if(handle){
					if(pcap_set_snaplen(handle, len) != 0){
						warning = true;
						reason = pcap_geterr(handle);
					}
				}else{
					warning = true;
					reason = "invalid handle";
				}
			}

			//before activate
			void set_promisc(int promisc){
				if(handle){
					if(pcap_set_promisc(handle, promisc) != 0){
						warning = true;
						reason = pcap_geterr(handle);
					}
				}else{
					warning = true;
					reason = "invalid handle";
				}
			}

			//before activate
			//radio frequency monitor
			void set_rfmon(int rfmon) noexcept{
				if(handle){
					if(pcap_can_set_rfmon(handle) != 1){
						warning = true;
						reason = "cannot set rfmon";
						return;
					}

					if(pcap_set_rfmon(handle, rfmon) != 0){
						warning = true;
						reason = pcap_geterr(handle);
						return;
					}
				}else{
					warning = true;
					reason = "invalid handle";
					return;
				}
			}

			//before activate
			void set_timeout(int timeout_ms){

				if(handle){
					if(pcap_set_timeout(handle, timeout_ms) != 0){
						warning = true;
						reason = pcap_geterr(handle);
						return;
					}
				}else{
					warning = true;
					reason = "invalid handle";
					return;
				}
				
			}

			//before activate
			void set_immediate_mode(int mode){
				if(handle){
					if(pcap_set_immediate_mode(handle, mode) != 0){
						warning = true;
						reason = pcap_geterr(handle);
						return;
					}
				}else{
					warning = true;
					reason = "invalid handle";
					return;
				}
			}

			//before activate
			void set_buffer_size(int size){
				if(handle){
					if(pcap_set_buffer_size(handle, size) != 0){
						warning = true;
						reason = pcap_geterr(handle);
						return;
					}
				}else{
					warning = true;
					reason = "invalid handle";
					return;
				}
			}

			//before activate
			void set_tstamp_type(int type){
				if(handle){
					if(pcap_set_tstamp_type(handle, type) != 0){
						warning = true;
						reason = pcap_geterr(handle);
						return;
					}
				}else{
					warning = true;
					reason = "invalid handle";
					return;
				}
			}

			//call from another thread
			void breakloop() noexcept{
				if(handle){
					pcap_breakloop(handle);
				}
			}

			//after activate
			//true on nonblock, false on warning
			bool getnonblock(int nonblock) noexcept{
				if(handle){
					if(pcap_getnonblock(handle, reason.data()) != 0){
						warning = true;
						reason = pcap_geterr(handle);
						return false;
					}
				}else{
					warning = true;
					reason = "invalid handle";
					return false;
				}

				return true;
			}

			//after activate
			void setnonblock(int nonblock) noexcept{
				if(handle){
					if(pcap_setnonblock(handle, nonblock, reason.data()) != 0){
						warning = true;
						reason = pcap_geterr(handle);
						return;
					}
				}else{
					warning = true;
					reason = "invalid handle";
					return;
				}
			}

			//after activate
			//PCAP_D_INOUT,PCAP_D_IN,PCAP_D_OUT
			void setdirection(pcap_direction_t dir){
				if(handle){
					if(pcap_setdirection(handle, dir) != 0){
						warning = true;
						reason = pcap_geterr(handle);
						return;
					}
				}else{
					warning = true;
					reason = "invalid handle";
					return;
				}
			}


		//raii
		public:
			pcap_ext_handle() noexcept{
				if(reason.capacity() < PCAP_ERRBUF_SIZE){
					reason.reserve(PCAP_ERRBUF_SIZE);
				}
			}

			~pcap_ext_handle() noexcept{close();}

	};


	class pcap_recv_worker{
		public:
			class worker_status{
				public:
					std::mutex mtx;
					std::condition_variable_any cv;

					bool has_jobs = false;
					bool done = false;

					auto get_status() noexcept{ return [this](){return has_jobs || done;};}
					void reset_status() noexcept{ has_jobs = false; }
			}status;

			//pkt element
			struct pkt_element{
				int ret_value;
				struct pcap_pkthdr pkthdr;
				std::vector<u_char> pktdata;
			};

			//handle manager
			core::pcap_ext_handle handle;

			//thread push data in, and main thread pop data out
			std::vector<struct pkt_element> pkt_vec;

			//dev(gived by main thread)
			std::string devname;
			enum handle_ctl{HANDLE_NONE, HANDLE_CREATE_AND_ACTIVATE, HANDLE_CLOSE} hc = HANDLE_NONE;

		public:
			void worker_func(std::stop_token stoken){

				while(true){
					bool wait = true;

					{//quit cond and event handle
						std::lock_guard<std::mutex> lock{status.mtx};

						//quit cond
						if(stoken.stop_requested())break;
						if(status.done)break;

						//job: handle notification
						if(hc == HANDLE_CREATE_AND_ACTIVATE){
							handle.create(devname.c_str());

							//between creating and activating
							handle.set_immediate_mode(1);
							handle.set_snaplen(65535);

							//handle.set_promisc(1);
							#if 0
							handle.set_rfmon(1);
							#endif

							handle.activate();

							//handle.setdirection(PCAP_D_INOUT);

							//after activating
							handle.setnonblock(1);

							hc = HANDLE_NONE;

						}else if(hc == HANDLE_CLOSE){
							handle.close();
							hc = HANDLE_NONE;
						}

						//wait ctl
						if(handle.is_ok()){
							//both creation and activation are done
							wait = false;
						}else{
							wait = true;
						}
					}

					//wait
					if(wait){
						std::unique_lock<std::mutex> lock{status.mtx};
						status.cv.wait(lock,stoken,status.get_status());

						//for next waiting
						status.reset_status();
					}else{

						//handle is_ok ture(create and activate)
						struct pcap_pkthdr *pkthdr_p = nullptr;
						const u_char *pktdata_p = nullptr;

						int ret = pcap_next_ex(handle.get_handle(), &pkthdr_p, &pktdata_p);

						if(pkthdr_p && pktdata_p){
							struct pkt_element buf;
							buf.ret_value = ret;
							buf.pkthdr = *pkthdr_p;
							buf.pktdata.assign(pktdata_p, pktdata_p + buf.pkthdr.caplen);

							{//push data
								std::lock_guard<std::mutex> lock{status.mtx};
								pkt_vec.push_back(std::move(buf));
							}
						}

					}

				}
			}

			void force_to_wake_up_worker(){
				std::unique_lock<std::mutex> lock(status.mtx);

				status.has_jobs = true;
				lock.unlock();
				status.cv.notify_all();
			}

			void try_to_wake_up_worker(){
				std::unique_lock<std::mutex> lock(status.mtx,std::try_to_lock_t{});

				if(lock.owns_lock()){
					status.has_jobs = true;
					lock.unlock();
					status.cv.notify_one();
				}
			}

			//need to wake up before thread join
			//manually call or use stop_token
			void release_worker(){
				std::unique_lock<std::mutex> lock(status.mtx);

				status.done = true;

				lock.unlock();
				status.cv.notify_all();
			}

			//run in a different thread than the worker thread
			void breakloop(){
				std::unique_lock<std::mutex> lock(status.mtx);

				if(handle.get_handle()){
					hc = core::pcap_recv_worker::HANDLE_CLOSE;
					handle.breakloop();

					lock.unlock();
					status.cv.notify_one();
				}
			}

		public:
			~pcap_recv_worker()=default;
	};


	class pcap_send_worker{
		public:
			class worker_status{
				public:
					std::mutex mtx;
					std::condition_variable_any cv;

					bool has_jobs = false;
					bool done = false;

					auto get_status() noexcept{ return [this](){return has_jobs || done;};}
					void reset_status() noexcept{ has_jobs = false; }
			}status;


			//thread send data out,and pop front, the main thread push data back
			std::deque<std::vector<u_char>> pkt_vec;

			//handle manager
			core::pcap_ext_handle handle;

			//dev(gived by main thread)
			std::string devname;
			enum handle_ctl{HANDLE_NONE, HANDLE_CREATE_AND_ACTIVATE, HANDLE_CLOSE} hc = HANDLE_NONE;

			//number of packet cannot send
			size_t error_number = 0;

		public:
			void worker_func(std::stop_token stoken){

				while(true){
					bool wait;
					
					{//quit cond
						std::lock_guard<std::mutex> lock{status.mtx};

						//quit cond
						if(stoken.stop_requested())break;
						if(status.done)break;

						//job: handle notification
						if(hc == HANDLE_CREATE_AND_ACTIVATE){
							handle.create(devname.c_str());

							#if 0
							handle.set_rfmon(1);
							#endif

							handle.activate();

							//handle.setdirection(PCAP_D_OUT);

							handle.setnonblock(1);

							hc = HANDLE_NONE;
						}else if(hc == HANDLE_CLOSE){
							handle.close();
							hc = HANDLE_NONE;
						}

						//wait ctl
						if(handle.is_ok() && pkt_vec.size()){
							//both creation and activation are done and thead has jobs
							wait = false;
						}else{
							wait = true;
						}
					}


					//wait
					if(wait){

						std::unique_lock<std::mutex> lock{status.mtx};
						status.cv.wait(lock,stoken,status.get_status());

						//for next waiting
						status.reset_status();
					}else{

						int err_send = 0;
						std::vector<u_char> pkt_buf;

						size_t retries = 0;

						//work
						while(true){

							{//lock area
								std::lock_guard<std::mutex> lock{status.mtx};
								if(pkt_vec.empty())break;
								if(stoken.stop_requested())break;
								if(status.done)break;

								if(retries >= 1500 && pkt_buf.size() > 1280 + 14){
									retries = 0;
									error_number++;
									err_send = 0;

									//SDL_Log("errsize: %ld",pkt_buf.size());
								}

								if(err_send == 0){//update just when send success or jump
									pkt_buf = std::move(pkt_vec.front());
									pkt_vec.pop_front();
								}else{
									retries++;

									if(pkt_buf.size() > 1280 + 14){
										std::this_thread::sleep_for(std::chrono::microseconds{10});
									}
								}
							}

							err_send = pcap_sendpacket(
									handle.get_handle(),
									pkt_buf.data(),
									pkt_buf.size()
							);


						}

					}

				}
			}

			void force_to_wake_up_worker(){
				std::unique_lock<std::mutex> lock(status.mtx);

				status.has_jobs = true;
				lock.unlock();
				status.cv.notify_all();
			}

			void try_to_wake_up_worker(){
				std::unique_lock<std::mutex> lock(status.mtx,std::try_to_lock_t{});

				if(lock.owns_lock()){
					status.has_jobs = true;
					lock.unlock();
					status.cv.notify_one();
				}
			}

			//need to wake up before thread join
			//manually call or use stop_token
			void release_worker(){
				std::unique_lock<std::mutex> lock(status.mtx);

				status.done = true;

				lock.unlock();
				status.cv.notify_all();
			}

		public:
			~pcap_send_worker()=default;
	};


	inline void hexstr_to_vectoruchar(const std::string &hexstr, std::vector<u_char> &data_buf){
		std::string valid_str;

		if(!data_buf.empty())data_buf.clear();

		for(auto &&e : hexstr){
			if(('0' <= e && e <= '9') || ('A' <= e && e <= 'F') || ('a' <= e && e <= 'f')){
				valid_str.push_back(e);
			}
		}

		for(size_t i=0;i+2 <= valid_str.size();i+=2){
			data_buf.push_back(static_cast<u_char>(std::stoi(valid_str.substr(i,2),nullptr,16)));
		}

	}

}
#endif
