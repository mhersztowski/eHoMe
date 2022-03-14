#ifndef EHLANG_H
#define EHLANG_H

#include "stdio.h"

#include "SymbolTable.h"

#include "Parser.h"
#include "Scanner.h"
#include "Logic.h"
#include <sys/timeb.h>
#include <wchar.h>

namespace EHLang {


class EHLang {

public:

    void initFromFile(const char* filename) {
        clean();
        wchar_t *fileName = coco_string_create(filename);
        scanner = new Scanner(fileName);
        parser = new Parser(scanner);
        parser->tab = new SymbolTable(parser);
        parser->gen = new CodeGenerator();
        parser->logic = new Logic();
        parser->logic->parser = parser;
        parser->logic->scanner = scanner;
        parser->logic->tab = parser->tab;
        parser->logic->gen = parser->gen;
        coco_string_delete(fileName);
    }

    void initFromUtf8Buffer(const char* data, int data_len) {
        clean();
        scanner = new Scanner((const unsigned char*) data, data_len);
        parser = new Parser(scanner);
        parser->tab = new SymbolTable(parser);
        parser->gen = new CodeGenerator();
        parser->logic = new Logic();
        parser->logic->parser = parser;
        parser->logic->scanner = scanner;
        parser->logic->tab = parser->tab;
        parser->logic->gen = parser->gen;
        //parser->Parse();
    }

    int parse() {
        if (parser == NULL)
            return 0;

        parser->Parse();
        if (parser->errors->count == 0)
            return 1;
        return 0;
    }

    int run() {
        if (parser == NULL)
            return 0;

        if (parser->errors->count == 0) {
            parser->gen->Decode();
            parser->gen->Interpret("Taste.IN");
            return 1;
        }

        return 0;
    }

    void clean() {
        if (parser) {
            if  (parser->tab) {
                delete parser->tab;
                parser->tab = NULL;
            }
            if (parser->gen) {
                delete parser->gen;
                parser->gen = NULL;
            }
            delete parser;
            parser = NULL;
       }

       if (scanner) {
           delete scanner;
           scanner = NULL;
       }

    }


    Scanner *scanner;
    Parser *parser;
};


}


#endif // EHLANG_H
