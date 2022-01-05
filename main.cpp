#include <iostream>
#include <App.h>
#include "ScannerManager.h"

int main(int argc, char ** argv)
{
    /*std::string data = "{\
    \"glossary\": {\
        \"title\": \"example glossary\",\
		\"GlossDiv\": {\
            \"title\": \"S\",\
			\"GlossList\": {\
                \"GlossEntry\": {\
                    \"ID\": \"SGML\",\
					\"SortAs\": \"SGML\",\
					\"GlossTerm\": \"Standard Generalized Markup Language\",\
					\"Acronym\": \"SGML\",\
					\"Abbrev\": \"ISO 8879:1986\",\
					\"GlossDef\": {\
                        \"para\": \"A meta-markup language, used to create markup languages such as DocBook.\",\
						\"GlossSeeAlso\": [\"GML\", \"XML\"]\
                    },\
					\"GlossSee\": \"markup\"\
                }\
            }\
        }\
    }\
}";
    uWS::App().get("/",[&data](auto * res, auto * req){
        res->writeHeader("Content-Type","application/json");
        res->write(data);
        res->end();
    }).listen(3000,[](auto * listen_socket){
        std::cout<<"listening on port 3000"<<std::endl;
    }).run();
    return 0;*/
    try{

    ScannerManager manager;
    //manager.refreshDeviceList();
    auto scanner = manager.getScanner("v4l:/dev/video0");
    std::cout<<scanner->getOptionsInJson()<<std::endl;
    scanner->setOption(2,"Color");
    std::cout<<scanner->getOptionsInJson()<<std::endl;
    scanner->scan();
    //manager._scanners[0].scan();

    }catch(std::runtime_error &e){
        std::cerr<<e.what()<<std::endl;
        return -1;
    }

}