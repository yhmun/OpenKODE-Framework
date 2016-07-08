/* --------------------------------------------------------------------------
 *
 *      File            ex_09.cpp
 *      Description     09. File system
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (C) 2010-2012 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com 
 * 
 * --------------------------------------------------------------------------
 * 
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2 of the License, or (at your option) any later version.
 * 
 *      This library is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *      Lesser General Public License for more details.
 * 
 *      You should have received a copy of the GNU Lesser General Public
 *      License along with this library in the file COPYING.LIB;
 *      if not, write to the Free Software Foundation, Inc.,
 *      59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 *
 * -------------------------------------------------------------------------- */

#include "main.h"

KDvoid xmMakeTempFile ( const KDchar* path, KDchar val, KDint count )
{
	KDFile*     file = KD_NULL;		
	KDint       i;

	file = kdFopen ( path, "w" );

	for ( i = 0; i < count; i++ )
	{
		kdPutc ( val + i, file );
	}

	kdFclose ( file );
}

KDvoid xmMakeTempFile2 ( const KDchar* path, const KDchar* str )
{
	KDFile*     file = KD_NULL;
	
	file = kdFopen ( path, "w" );

	kdFwrite ( str, kdStrlen ( str ), 1, file );

	kdFclose ( file );
}

KDvoid xmEx_kdFopen ( KDvoid )
{
	KDchar      path[] = "/data/Ex_kdFopen.txt";
	KDFile*     file   = KD_NULL;		
		
	xmMakeTempFile ( path, 'a', 26 );

	/*
		함수 : 파일 열기 	
		
		인수 : 파일 패스 ( 읽기 전용 = "/", "/res", "/removable", 쓰기 허용 = "/data", "/tmp", "/native" )
			   파일 모드 ( "r", "w", "a", 그 외 문서 참고 )

		반환 : 성공 ( 파일 핸들 )
			   실패 ( KD_NULL )
	*/
	
	file = kdFopen ( path, "r" );
	kdAssert ( file );

	kdFclose ( file );	
	kdLogMessagefKHR ( "kdFopen    : '%s'", path );
}

KDvoid xmEx_kdFclose ( KDvoid )
{
	KDchar      path[] = "/data/Ex_kdFclose.txt";
	KDFile*     file   = KD_NULL;	
	KDint       ret    = 0;
		
	xmMakeTempFile ( path, 'a', 26 );

	file = kdFopen ( path, "r" );

	/*
		함수 : 파일 닫기 	

		인수 : 파일 핸들

		반환 : 성공 ( 0 )
			   실패 ( KD_EOF )
	*/
	
	ret = kdFclose ( file );	
	kdAssert ( !ret );
	
	kdLogMessagefKHR ( "kdFclose   : '%s'", path );
}

KDvoid xmEx_kdFflush ( KDvoid )
{
	KDchar      path[]  = "/data/Ex_kdFflush.txt";
	KDchar      buf[50] = ""; 
	KDFile*     file    = KD_NULL;
	KDint       ret	    = 0;
	KDint       i;

	xmMakeTempFile ( path, 'a', 26 );

	file = kdFopen ( path, "r+" );	
	for ( i = 0;  i < 10; i++ )
	{
		kdPutc ( '0' + i, file );
	}

	/*
		함수 : 파일 FLUSH 	
		
		인수 : 파일 핸들

		반환 : 성공 ( 0 )
			   실패 ( KD_EOF )
	*/

	ret = kdFflush ( file );	
	kdAssert ( !ret );

	kdFgets ( buf, 30, file );
	kdLogMessagefKHR ( "kdFflush   : '%s', buf = '%s'", path, buf );

	kdFclose ( file );
}
KDvoid xmEx_kdFread ( KDvoid )
{
	KDchar      path[]  = "/data/Ex_kdFread.txt";
	KDchar      buf[50] = ""; 
	KDFile*     file    = KD_NULL;
	KDint       ret	    = 0;
	
	xmMakeTempFile ( path, 'a', 26 );

	file = kdFopen ( path, "r" );

	/*
		함수 : 파일 읽기 	
		
		인수 : 버퍼
			   사이즈
			   횟수
			   파일 핸들

		반환 : 읽은 횟수
	*/

	ret = kdFread ( buf, 13, 2, file );
	kdAssert ( ret == 2 );

	kdFclose( file );	
	kdLogMessagefKHR ( "kdFread    : '%s' , buf = '%s'", path, buf );
}

KDvoid xmEx_kdFwrite ( KDvoid )
{
	KDchar      path[] = "/data/Ex_kdFwrite.txt";
	KDFile*     file   = KD_NULL;
	KDint       ret    = 0;
	
	xmMakeTempFile ( path, 'a', 26 );

	file = kdFopen ( path, "a" );

	/*
		함수 : 파일 쓰기 	
		
		인수 : 버퍼
			   사이즈
			   횟수
			   파일 핸들

		반환 : 읽은 횟수
	*/

	ret = kdFwrite ( "012345", 6, 1, file );
	kdAssert ( ret == 1 );

	kdFclose ( file );	
	kdLogMessagefKHR ( "kdFwrite   : '%s'", path );
}

KDvoid xmEx_kdGetc ( KDvoid )
{
	KDchar      path[] = "/data/Ex_kdGetc.txt";	
	KDFile*     file   = KD_NULL;
	KDchar      ret    = 0;
	
	xmMakeTempFile ( path, 'a', 26 );

	file = kdFopen ( path, "r" );

	/*
		함수 : 파일 바이트 읽기 	
		
		인수 : 파일 핸들

		반환 : 읽은 바이트
	*/

	ret = kdGetc ( file );
	kdAssert ( ret == 'a' );

	kdFclose ( file );	
	kdLogMessagefKHR ( "kdGetc     : '%s'  , char = '%c'", path, ret );
}

KDvoid xmEx_kdPutc ( KDvoid )
{
	KDchar      path[] = "/data/Ex_kdPutc.txt";	
	KDFile*     file   = KD_NULL;
	KDint       ret    = 0;
	
	xmMakeTempFile ( path, 'a', 26 );

	file = kdFopen ( path, "a" );

	/*
		함수 : 파일 바이트 쓰기 	
		
		인수 : 쓰기 바이트
			   파일 핸들

		반환 : 성공 ( 쓰기 바이트 )
		       실패 ( KD_EOF )
	*/

	ret = kdPutc ( '0', file );
	kdAssert ( ret == '0' );

	kdFclose ( file );	
	kdLogMessagefKHR ( "kdPutc     : '%s'", path );
}

KDvoid xmEx_kdFgets ( KDvoid )
{
	KDchar      path[]  = "/data/Ex_kdFgets.txt";	
	KDchar      buf[10] = "";
	KDFile*     file    = KD_NULL;	
	KDchar*	    ret	    = 0;
	
	xmMakeTempFile2 ( path, "This\nis\ntest\n" );

	file = kdFopen ( path, "r" );

	/*
		함수 : 문자열 라인 읽기	
		
		인수 : 버퍼
			   버퍼 사이즈 (리밋)
			   파일 핸들

		반환 : 읽은 문자열
	*/

	ret = kdFgets ( buf, 10, file );
	kdAssert ( kdStrcmp ( ret, "This" ) );

	ret = kdFgets ( buf, 10, file );
	kdAssert ( kdStrcmp ( ret, "is" ) );

	ret = kdFgets ( buf, 10, file );
	kdAssert ( kdStrcmp ( ret, "test" ) );

	ret = kdFgets ( buf, 10, file );
	kdAssert ( ret == KD_NULL );
	
	kdFclose ( file );	
	kdLogMessagefKHR ( "kdFgets    : '%s'", path );
}

KDvoid xmEx_kdFEOF ( KDvoid )
{
	KDchar      path[]  = "/data/Ex_kdFEOF.txt";	
	KDchar      buf[50] = "";
	KDFile*     file    = KD_NULL;	
	KDint       i       = 0;
	
	xmMakeTempFile ( path, '0', 5 );

	file = kdFopen ( path, "r" );

	/*
		함수 : 파일 끝 확인	
		
		인수 : 파일 핸들

		반환 : 파일 끝이면 KD_EOF 그 외는 0
	*/

	while ( !kdFEOF ( file ) )
	{
		buf[i++] = kdGetc ( file );
	}	
	kdAssert ( kdStrcmp ( buf, "01234" ) );

	kdFclose ( file );
	kdLogMessagefKHR ( "kdFEOF     : '%s'  , buf = '%s'", path, buf );
}

KDvoid xmEx_kdFerror ( KDvoid )
{
	KDchar      path[] = "/data/Ex_kdFerror.txt";	
	KDFile*     file   = KD_NULL;	
	KDint       ret    = 0;
	
	xmMakeTempFile ( path, 'a', 26 );

	file = kdFopen ( path, "r" );
	kdPutc ( '0', file );

	/*
		함수 : 파일 에러 체크	
		
		인수 : 파일 핸들

		반환 : 에러 발생시 0이 아닌값
	*/

	ret = kdFerror ( file );
	//kdAssert ( ret );

	kdFclose( file );
	kdLogMessagefKHR ( "kdFerror   : '%s'", path );
}

KDvoid xmEx_kdClearerr ( KDvoid )
{
	KDchar      path[] = "/data/Ex_kdClearerr.txt";	
	KDFile*     file   = KD_NULL;	
	KDint       ret    = 0;	
	
	xmMakeTempFile ( path, 'a', 26 );

	file = kdFopen ( path, "r" );
	kdPutc ( '0', file );
	ret = kdFerror ( file );

	/*
		함수 : 파일 에러 클리어	
		
		인수 : 파일 핸들
	*/

	kdClearerr ( file );
	ret = kdFerror ( file );
	kdAssert ( !ret );
	
	kdFclose ( file );
	kdLogMessagefKHR ( "kdClearerr : '%s'", path );
}

KDvoid xmEx_kdFseek ( KDvoid )
{
	KDchar      path[]  = "/data/Ex_kdFseek.txt";	
	KDchar      buf[10] = "";
	KDFile*     file    = KD_NULL;	

	xmMakeTempFile ( path, '0', 10 );

	file = kdFopen ( path, "r" );

	/*
		함수 : 파일 탐색	
		
		인수 : 파일 핸들
			   위치 
			   시작 위치 : KD_SEEK_SET, KD_SEEK_CUR, KD_SEEK_END

		반환 : 성공 ( 0 )
			   실패 ( -1 )
	*/

	kdFseek ( file, 5, KD_SEEK_SET );
	kdFread ( buf, 5, 1, file );
	kdAssert ( !kdStrcmp ( buf, "56789" ) );

	kdFclose ( file );
	kdLogMessagefKHR ( "kdFseek    : '%s',  buf = %s", path, buf );
}

KDvoid xmEx_kdFtell ( KDvoid )
{
	KDchar      path[] = "/data/Ex_kdFtell.txt";	
	KDFile*     file   = KD_NULL;	
	KDoff       ret    = 0;	

	xmMakeTempFile ( path, '0', 10 );

	file = kdFopen ( path, "r" );
	kdFseek ( file, 7, KD_SEEK_SET );
	
	/*
		함수 : 파일 위치	
		
		인수 : 파일 핸들
			   
		반환 : 성공 ( 파일 위치 )
			   실패 ( (KDoff) -1 )
			   
	*/

	ret = kdFtell ( file );
	kdAssert ( ret == 7 );

	kdFclose ( file );
	kdLogMessagefKHR ( "kdFtell    : '%s',  off = %d", path, ret );
}

KDvoid xmEx_kdMkdir(void)
{
	KDchar      path[] = "/data/Ex_kdMkdir";	
	KDint       ret    = 0;

	kdRmdir ( path );

	/*
		함수 : 디렉토리 생성
		
		인수 : 디렉토리 패스 ( 제한 : '/', '/res', '/removable' )
			   
		반환 : 성공 (  0 )
			   실패 ( -1 )
			   
	*/

	ret = kdMkdir ( path );
	kdAssert ( !ret );

	kdLogMessagefKHR ( "kdMkdir    : '%s'", path );
}

KDvoid xmEx_kdRmdir ( KDvoid )
{
	KDchar      path[] = "/data/Ex_kdRmdir";	
	KDint       ret    = 0;

	kdMkdir ( path );

	/*
		함수 : 디렉토리 삭제
		
		인수 : 디렉토리 패스 ( 제한 : '/', '/res', '/removable' )
			   
		반환 : 성공 (  0 )
			   실패 ( -1 )
			   
	*/

	ret = kdRmdir ( path );
	kdAssert ( !ret );

	kdLogMessagefKHR ( "kdRmdir    : '%s'", path );
}

KDvoid xmEx_kdRename ( KDvoid )
{
	KDchar      path1[] = "/data/abc";
	KDchar      path2[] = "/data/Ex_kdRename";		
	KDint       ret     = 0;
	
	kdMkdir ( path1 );
	kdRmdir ( path2 );

	/*
		함수 : 파일 이름 변경 (디렉토리 포함)
		
		인수 : 소스 패스 ( 제한 : '/', '/res', '/removable' )
		       대상 패스 ( 제한 : '/', '/res', '/removable' )
			   
		반환 : 성공 (  0 )
			   실패 ( -1 )
			   
	*/

	ret = kdRename ( path1, path2 );
	kdAssert ( !ret );

	kdLogMessagefKHR ( "kdRename   : '%s'", path2 );
}

KDvoid xmEx_kdRemove ( KDvoid )
{
	KDchar      path[] = "/data/Ex_kdRemove.txt";		
	KDint       ret    = 0;

	xmMakeTempFile ( path, '0', 10 );

	/*
		함수 : 파일 삭제
		
		인수 : 파일 패스 ( 제한 : '/', '/res', '/removable' )
			   
		반환 : 성공 (  0 )
			   실패 ( -1 )
			   
	*/

	ret = kdRemove ( path );
	kdAssert ( !ret );

	kdLogMessagefKHR ( "kdRemove   : '%s'", path );
}

KDvoid xmEx_kdTruncate(void)
{
	KDchar      path[] = "/data/Ex_kdTruncate.txt";		
	KDint       ret    = 0;

	xmMakeTempFile ( path, '0', 10 );

	/*
		함수 : 파일 길이 변경
		
		인수 : 파일 패스 ( 제한 : '/', '/res', '/removable' )
			   파일 길이 
			   
		반환 : 성공 (  0 )
			   실패 ( -1 )
			   
	*/

	ret = kdTruncate ( path, 20 );
	//kdAssert ( !ret );

	kdLogMessagefKHR ( "kdTruncate : '%s'", path );
}

KDvoid xmEx_kdStat ( KDvoid )
{
	KDchar      path[] = "/data/Ex_kdStat.txt";			
	KDStat      stat;

	xmMakeTempFile ( path, 'a', 26 );

	/*
		함수 : 파일 정보 
		
		인수 : 파일 패스 ( 제한 : '/', '/removable' )
			   정보 구조체
			   
		반환 : 성공 (  0 )
			   실패 ( -1 )
			   
	*/

	kdStat ( path, &stat );
	kdAssert ( stat.st_size == 26 );
	
	kdLogMessagefKHR ( "kdStat     : '%s' , size = %d", path, stat.st_size );
}

KDvoid xmEx_kdFstat ( KDvoid )
{
	KDchar      path[] = "/data/Ex_kdFstat.txt";	
	KDFile*     file   = KD_NULL;
	KDint       ret    = 0;
	KDStat      stat;

	xmMakeTempFile ( path, 'a', 26 );

	file = kdFopen ( path, "r" );

	/*
		함수 : 파일 정보 
		
		인수 : 파일 핸들
			   정보 구조체
			   
		반환 : 성공 (  0 )
			   실패 ( -1 )
			   
	*/

	ret = kdFstat ( file, &stat );
	if ( ret == -1 && kdGetError ( ) == KD_ENOSYS )
	{
		kdLogMessagefKHR ( "kdFstat    : not supported api." );
		return;
	}
	kdAssert ( stat.st_size == 26 );
	
	kdFclose( file );
	kdLogMessagefKHR ( "kdFstat    : '%s', size = %d", path, stat.st_size );	
}

KDvoid xmEx_kdAccess ( KDvoid )
{
	KDchar      path[] = "/data/Ex_kdAccess.txt";		
	KDint       ret    = 0;		

	xmMakeTempFile ( path, 'a', 26 );

	/*
		함수 : 파일 권한 
		
		인수 : 파일 패스 ( 제한 : '/', '/res', '/removable' )
			   파일 모드 ( KD_R_OK, KD_W_OK, KD_X_OK (조합) )
			   
		반환 : 성공 (  0 )
			   실패 ( -1 )
			   
	*/

	ret = kdAccess ( path, KD_R_OK );
	kdAssert ( !ret || kdGetError ( ) == KD_ENOSPC );

	kdLogMessagefKHR ( "kdAccess   : '%s'", path );
}

KDvoid xmEx_kdOpenDir ( KDvoid )
{
	KDchar      path[] = "/data";
	KDDir*      dir	   = 0;
			
	/*
		함수 : 디렉토리 열기
		
		인수 : 디렉토리 패스 ( 제한 : '/' )
			   			   
		반환 : 디렉토리 핸들
			   
	*/

	dir = kdOpenDir ( path );
	kdAssert ( dir );

	kdCloseDir ( dir );
	kdLogMessagefKHR ( "kdOpenDir  : '%s'", path );
}
KDvoid xmEx_kdReadDir ( KDvoid )
{
	KDchar      path[] = //"/data";
						"/res/Jpeg";
	KDDir*      dir	   = 0;
	KDDirent*   dirent = 0;
			
	dir = kdOpenDir ( path );

	/*
		함수 : 디렉토리 읽기
		
		인수 : 디렉토리 핸들
			   			   
		반환 : 디렉토리 정보
			   
	*/

	while ( ( dirent = kdReadDir ( dir ) ) )
	{
		kdLogMessagefKHR ( "kdReadDir  : '%s'", dirent->d_name );
	}
	
	kdCloseDir ( dir );	
}

KDvoid xmEx_kdCloseDir(void)
{
	KDchar      path[] = "/data";
	KDDir*      dir	   = 0;
	KDint       ret    = 0;

	dir = kdOpenDir ( path );

	/*
		함수 : 디렉토리 닫기
		
		인수 : 디렉토리 핸들
			   			   
		반환 : 성공 (  0 )
			   
	*/

	ret = kdCloseDir ( dir );
	kdAssert ( !ret );

	kdLogMessagefKHR ( "kdCloseDir : '%s'", path );
}

KDvoid xmEx_kdGetFree ( KDvoid )
{
	KDchar      path[] = "/data";		
	KDoff       ret    = 0;	

	/*
		함수 : 디렉토리 남은 공간
		
		인수 : 디렉토리 패스 ( 제한 : '/', '/removable' )
			   			   
		반환 : 남은 공간
			   
	*/

	ret = kdGetFree ( path );
	kdAssert ( ret != -1L || kdGetError ( ) == KD_ENOSPC );

	kdLogMessagefKHR ( "kdGetFree  : '%s', off = %u Mbytes", path, ret / 1024 / 1024 );
}

KDvoid xmEx_xmGetNativePath ( KDvoid )
{
	/*
		함수 : 절대 경로 구하기
		
		인수 : 가상 경로, 가상 경로 반환자 ( KD_NULL 허용 )
			   			   
		반환 : 절대 경로 ( kdFree 함수로 반드시 해제 해야 한다. )
			   
	*/

	KDchar	vpaths[][16] = { "/res", "/data", "/tmp", "/removable", "/storage", "/native", "/" };
	KDchar* path;
	KDint   i;

	for ( i = 0; i < 7; i++ )
	{
		path = xmGetNativePath ( vpaths [ i ], KD_NULL );
		kdLogMessagefKHR ( "xmGetNativePath : vir = '%s', full = '%s'", vpaths [ i ], path );	
		kdFree ( path );
	}
}

KDvoid xmExample_09 ( KDvoid )
{	
	kdLogMessage ( "Example 09. File system\n\n" );

	xmEx_kdFopen    ( );
	xmEx_kdFclose   ( );
	xmEx_kdFflush   ( );	
	xmEx_kdFread    ( );
	xmEx_kdFwrite   ( );
	xmEx_kdGetc     ( );
	xmEx_kdPutc     ( );
	xmEx_kdFgets    ( );
	xmEx_kdFEOF     ( );
	xmEx_kdFerror   ( );
	xmEx_kdClearerr ( );
	xmEx_kdFseek    ( );	
	xmEx_kdFtell    ( );
	xmEx_kdMkdir    ( );
	xmEx_kdRmdir    ( );
	xmEx_kdRename   ( );
	xmEx_kdRemove   ( );
	xmEx_kdTruncate ( );
	xmEx_kdStat     ( );
	xmEx_kdFstat    ( );
	xmEx_kdAccess   ( );
	xmEx_kdOpenDir  ( );
	xmEx_kdReadDir  ( );
	xmEx_kdCloseDir ( );
	xmEx_xmGetNativePath ( );

#if !defined ( WIN32 )
	xmEx_kdGetFree  ( );        // Win32_x86 : Released ( libKD ) Build Error
#endif
}
