#ifndef __irsl_display_util_h__
#define __irsl_display_util_h__

#include <pthread.h>
#include <vector>
#include <queue>
#include <boost/circular_buffer.hpp>
#include "display_base.h"
#include "utils.h"

// list表示バッファの大きさ
#define LINE_SIZE 2048
#define MAX_LINE  1024

namespace irsl_common_utils
{

// for stdoutに出力
class StandardDisplay : public DisplayBase {
    std::ostream m_nullstream;

public:
    DisplayStandard() : m_nullstream( 0 ) {}
    virtual void printf( char* format, ...) override;
    virtual void printf( const char* format, ...) override;
    virtual void perror( const char* msg ) override;
    // なにもしない
    virtual void output( unsigned int index, char* format, ...) override;
    // なにもしない
    virtual std::ostream& output_out( unsigned int index ) override;
    // なにもしない
    virtual unsigned int addOutput() override;
    // なにもしない
    virtual void flush() override;
};

// 表示クラス
class ListDisplay : public DisplayBase
{
#if 0
    PthreadMutex m_printfmutex;
    PthreadMutex m_outputmutex;

    class OutputStreamBuf : public std::streambuf{
        char m_wbuf[LINE_SIZE];
        ListDisplay* m_output;
        int m_index;
    public:
        OutputStreamBuf( ListDisplay* output, int index );
        int sync();
    };
    struct Output{
        OutputStreamBuf m_outputStream;
        std::ostream outputStream;
        char* buf;
        Output( ListDisplay* output, int index );
        ~Output();
    };
    std::vector<Output*>  m_outputs;
    boost::circular_buffer<char*> m_displaylines;
    ListDisplay(): m_displaylines(1024){}
    enum { MAX_BUF = 128 };
    void puts( char* buf );
    int pre_size;
    void output_buf( unsigned int index, char* );
#endif

public:
    // リストの長さ
    ListDisplay( int num );
    virtual ~ListDisplay();
    // ダンプに一行表示
    virtual void printf( char* format, ...) override;
    virtual void printf( const char* format, ...) override;
    // perrorをダンプ
    virtual void perror( const char* msg ) override;
    // list表示を更新
    virtual void output( unsigned int index, char* format, ...) override;
    // listストリーム
    virtual std::ostream& output_out( unsigned int index ) override;
    // list表示を追加
    virtual unsigned int addOutput() override;
    // 描画
    virtual void flush() override;

private:
    class Impl;
    Impl *impl;
};

class MatrixDisplay : public ListDisplay
{
    MatrixDisplay (int row, int col);
    virtual ~MatrixDisplay();
};

}
#endif
