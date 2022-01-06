#include <iostream>
//#include <App.h>
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
    //manager.refreshDeviceList(); Can be used to find devices, but takes some time

    auto scanner = manager.getScanner("net:192.168.1.9:hpaio:/usb/HP_LaserJet_Pro_MFP_M125a?serial=CNB6H9PBMM");

    scanner->setOption(2,std::string("Color"));
    scanner->setOption(3,std::string("100"));
    scanner->setOption(8,std::string("None"));

    std::cout<<scanner->getOptionsInJson().dump()<<std::endl;
    if(argc<2){
        return 0;
    }
    scanner->scan(std::string(argv[1]));

    }catch(std::runtime_error &e){
        std::cerr<<e.what()<<std::endl;
        return -1;
    }
}