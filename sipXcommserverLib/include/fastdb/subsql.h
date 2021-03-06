//-< SUBSQL.H >------------------------------------------------------*--------*
// FastDB                    Version 1.0         (c) 1999  GARRET    *     ?  *
// (Main Memory Database Management System)                          *   /\|  *
//                                                                   *  /  \  *
//                          Created:     20-Nov-98    K.A. Knizhnik  * / [] \ *
//                          Last update: 10-Dec-98    K.A. Knizhnik  * GARRET *
//-------------------------------------------------------------------*--------*
// Interactive data manipulation language (subset of SQL)
//-------------------------------------------------------------------*--------*

#ifndef __SUBSQL_H__
#define __SUBSQL_H__

BEGIN_FASTDB_NAMESPACE

enum SubSqlTokens {
    tkn_alter = tkn_last_token,
    tkn_array,
    tkn_autoincrement,
    tkn_autocommit,
    tkn_backup,
    tkn_bool,
    tkn_commit,
    tkn_compactify,
    tkn_count,
    tkn_create,
    tkn_delete,
    tkn_describe,
    tkn_drop,
    tkn_exit,
    tkn_export,
    tkn_hash,
    tkn_help,
    tkn_http,
    tkn_import,
    tkn_index,
    tkn_int1,
    tkn_int2,
    tkn_int4,
    tkn_int8,
    tkn_inverse,
    tkn_memory,
    tkn_of,
    tkn_off,
    tkn_on,
    tkn_open,
    tkn_profile,
    tkn_real4,
    tkn_real8,
    tkn_rectangle,
    tkn_reference,
    tkn_rollback,
    tkn_server,
    tkn_set,
    tkn_stop,
    tkn_semi,
    tkn_show,
    tkn_to,
    tkn_update,
    tkn_values,
    tkn_version
};



class dbList {
  public:
    enum NodeType {
        nInteger,
        nBool,
        nReal,
        nString,
        nTuple,
        nAutoinc,
        nIdentifier
    };

    dbList* next;
    int     type;
    union {
        bool  bval;
        db_int8  ival;
        real8 fval;
        char* sval;
        struct {
            int     nComponents;
            dbList* components;
        } aggregate;
    };

    ~dbList() {
        if (type == nTuple) {
            dbList* list = aggregate.components;
            while (list != NULL) {
                dbList* tail = list->next;
                delete list;
                list = tail;
            }
        } else if (type == nString || type == nIdentifier) {
            delete[] sval;
        }
    }

    dbList(int type) {
        this->type = type;
        next = NULL;
    }
};


struct tableField {
    char* name;
    char* refTableName;
    char* inverseRefName;
    int   type;

    tableField() { name = refTableName = inverseRefName = NULL; }
    ~tableField() { delete[] name; delete[] refTableName; delete[] inverseRefName; }
};

class dbUpdateElement {
  public:
    dbUpdateElement*   next;
    dbFieldDescriptor* field;
    dbExprNode*        value;
    char*              strValue;

    dbUpdateElement() {
        next = NULL;
        strValue = NULL;
        value = NULL;
    }
    ~dbUpdateElement() {
        delete[] strValue;
        delete value;
    }
};


#define MAX_HISTORY_SIZE 16

class dbXmlScanner {
  public:
    enum {
        MaxIdentSize = 256
    };
    enum token {
        xml_ident,
        xml_sconst,
        xml_iconst,
        xml_fconst,
        xml_lt,
        xml_gt,
        xml_lts,
        xml_gts,
        xml_eq,
        xml_eof,
        xml_error
    };
    dbXmlScanner(FILE* f) {
        in = f;
        sconst = new char[size = 1024];
        line = 1;
        pos = 0;
    }
    token scan();

    char* getString() {
        return sconst;
    }

    char* getIdentifier() {
        return ident;
    }

    size_t  getStringLength() {
        return slen;
    }

    db_int8 getInt() {
        return iconst;
    }

    double getReal() {
        return fconst;
    }

    bool expect(int sourcePos, token expected) {
        token tkn = scan();
        if (tkn != expected) {
            fprintf(stderr, "subsql.cpp:%d: line %d, column %d: Get token %d instead of expected token %d\n",
                    sourcePos, line, pos, tkn, expected);
            return false;
        }
        return true;
    }

    bool expect(int sourcePos, char* expected) {
        token tkn = scan();
        if (tkn != xml_ident) {
            fprintf(stderr, "subsql.cpp:%d: line %d, column %d: Get token %d instead of expected identifier\n",
                    sourcePos, line, pos, tkn);
            return false;
        }
        if (strcmp(ident, expected) != 0) {
            fprintf(stderr, "subsql.cpp:%d: line %d, column %d: Get tag '%s' instead of expected '%s'\n",
                    sourcePos, line, pos, ident, expected);
            return false;
        }
        return true;
    }

  private:
    int   get();
    void  unget(int ch);

    int       line;
    int       pos;
    FILE*     in;
    char*     sconst;
    size_t    size;
    size_t    slen;
    db_int8   iconst;
    double    fconst;
    char      ident[MaxIdentSize];
};

class dbTmpAllocator {
    enum {
        CHUNK_SIZE = 4096
    };
    struct Chunk {
        Chunk* next;
        Chunk* prev; // is not used, added for alignment
    };
    Chunk* curr;
    size_t used;

  public:
    dbTmpAllocator() {
        curr = NULL;
        used = CHUNK_SIZE;
    }

    ~dbTmpAllocator() {
        reset();
    }

    void reset() {
        Chunk *c, *next;
        for (c = curr; c != NULL; c = next) {
            next = c->next;
            dbFree(c);
        }
        curr = NULL;
        used = CHUNK_SIZE;
    }


    void* alloc(size_t size) {
        size = DOALIGN(size, 8);
        if (size > CHUNK_SIZE/2) {
            Chunk* newChunk = (Chunk*)dbMalloc(size + sizeof(Chunk));
            if (curr != NULL) {
                newChunk->next = curr->next;
                curr->next = newChunk;
            } else {
                curr = newChunk;
                newChunk->next = NULL;
                used = CHUNK_SIZE;
            }
            return newChunk+1;
        } else if (size <= CHUNK_SIZE - used) {
            used += size;
            return (char*)curr + used - size;
        } else {
            Chunk* newChunk = (Chunk*)dbMalloc(CHUNK_SIZE);
            used = sizeof(Chunk) + size;
            newChunk->next = curr;
            curr = newChunk;
            return newChunk+1;
        }
    }
};

class dbSubSql : public dbDatabase {
  private:
    int   pos;
    int   line;
    int   tknPos;
    char* buf;
    int   buflen;
    FILE* in;
    bool  opened;
    db_int8  ival;
    real8 fval;
    char* name;

    oid_t*  oidMap;
    oid_t   oidMapSize;

    dbTmpAllocator tmpAlloc;

    static char* prompt;

    dbTableDescriptor* droppedTables;
    dbTableDescriptor* existedTables;

    dbQuery query;
    dbCompiler compiler;

    int      ungetToken;
    bool     autocommit;

    bool     dotIsPartOfIdentifier;
    char*    dateFormat;

    dbThread httpServerThread;
    HTTPapi* httpServer;
    bool     httpServerRunning;
    char*    queryHistory[MAX_HISTORY_SIZE];
    unsigned historyUsed;
    unsigned historyCurr;
    static void thread_proc httpServerThreadProc(void* arg);

    void deleteColumns(dbFieldDescriptor* columns);

    void httpServerLoop();

    void startHttpServer(char const* address);
    void stopHttpServer(char const* address);

    void handleError(dbErrorClass error, char const* msg = NULL,  int arg = 0);

    void error(char const* msg);
    void warning(char const* msg);

    void profile();

    int  get();
    void unget(int ch);
    int  scan();
    bool parse();

    bool expect(char* expected, int token);

    void recovery();

    void exportDatabase(FILE* out);
    bool importDatabase(FILE* in);

    void exportScheme(FILE* out);
    void exportClass(FILE* out, char* name, dbFieldDescriptor* fieldList);

    oid_t mapId(long id);
    bool importField(char* terminator, dbFieldDescriptor* fd, byte* rec, dbXmlScanner& scanner);
    bool importRecord(char* terminator, dbFieldDescriptor* fieldList, byte* rec, dbXmlScanner& scanner);
    void insertRecord(dbTableDescriptor* desc, oid_t oid, void const* record);

    bool isValidOid(oid_t oid);

    void dumpRecord(byte* record, dbFieldDescriptor* first);
    static int calculateRecordSize(dbList* list, int offs,
                                   dbFieldDescriptor* first);
    int  initializeRecordFields(dbList* node, byte* dst, int offs,
                                      dbFieldDescriptor* first);
    bool insertRecord(dbList* list, dbTableDescriptor* desc);
    bool readCondition();
    int  readExpression();
    int  readValues(dbList** chain);
    bool updateFields(dbAnyCursor* cursor, dbUpdateElement* elems);
    bool updateTable(bool create);
    int  parseType(char*& refTableName, char*& inverseRefName);
    int  updateRecords(dbTableDescriptor* desc, dbList *fields, dbList *values, dbAnyCursor &cursor, byte *buf);
    dbFieldDescriptor* readFieldName();

  public:
    void run(int firstParam, int argc, char* argv[]);
    void selectionPage(WWWconnection& con);
    void queryPage(WWWconnection& con);
    void defaultPage(WWWconnection& con);

    dbSubSql(dbAccessType accessType);
    virtual~dbSubSql();
};


END_FASTDB_NAMESPACE

#endif
