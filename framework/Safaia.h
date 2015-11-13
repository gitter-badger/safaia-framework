#include <iostream>
#include <vector>
#include <queue>
#include <sys/socket.h>
#include <netinet/in.h>
#include "Log.h"
#include "Route.h"
#include "Ecp.h"

#define BUF_LEN 1028

namespace Safaia{

    class Server {

        bool soft_stop = false;
        std::vector<Route> vec_routes;
        std::queue<Req> que_reqs;

        int sockfd, err, newfd;
        struct sockaddr_in addr;

        // IO Loop
        void loop(){
            newfd = accept(sockfd, NULL, NULL);
            log.info("Server", "Recieved a Request");

            // TODO: Handle Message
            char buf[BUF_LEN];
            read(newfd, buf, BUF_LEN);
            if (!strncmp(buf, "GET", 3)){
                std::string header = "HTTP/1.1 200 OK\r\nContent-type: text/html\r\n\r\n";
                std::string content = "<html><body><h1> Hello Safaia! </h1></body></html>\n";
                write(newfd, header.c_str(), header.length());
                write(newfd, content.c_str(), content.length());
            }
            // Close Connection
            close(newfd);
        }

    public:
        Log log = Log();
        int port = 21411;
        int thread = 4;

        Server(){

        }

        // Route Definition
        void add_route(Route route){
            vec_routes.push_back(route);
        }

        // Start running the server
        void run(){
            // Initializing
            log.info("Server","Safaia Server is initializing");

            // Establish TCP Socket
            sockfd = socket(AF_INET, SOCK_STREAM, 0);
            if (sockfd < 0) {
                log.error("Server", "Socket Creation Failed!");
                exit(0);
            } else {
                log.info("Server", "Socket Creation Succeeded!");
            }

            memset(&addr, 0, sizeof(addr));
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);
            addr.sin_addr.s_addr = INADDR_ANY;

            // Binding Address and Port
            if(bind(sockfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in))){
                log.error("Server", "Port Binding Failed at Port" + std::to_string(port));
                exit(0);
            } else {
                log.info("Server", "Port Binding Succeeded!");
            }

            // Start Listening
            listen(sockfd, 128);

            // Server Started
            log.info("Server","Safaia Server has Started at Port " + std::to_string(port));

            while (!soft_stop){
                //IO Loop
                loop();
            }
            log.info("Server","Safaia Server has Stopped");
        }

        // Soft Stop running the server
        void stop(){
            soft_stop = true;
            log.info("Server","Safaia Server is Stopping");
        }

        // Hard Stop running the server
        void stop_force(){
            log.warning("Server","Safaia Server is Force Stopping");
            delete(this);
        }

    };
}