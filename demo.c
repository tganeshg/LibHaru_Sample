/*
 * << Haru Free PDF Library 2.0.0 >> -- line_demo.c
 *
 * Copyright (c) 1999-2006 Takeshi Kanno <takeshi_kanno@est.hi-ho.ne.jp>
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.
 * It is provided "as is" without express or implied warranty.
 *
 * Custamized By : Ganesh
 */

/* Includes */
#include "demo.h"

/* Globals */

const char *tableHeadings[] = { "S.No",
								"Parameter",
								"Value",
								"Unit",
								"Updated On",
								"Remarks",
								NULL};

jmp_buf env;

/* Functions */

void error_handler(	HPDF_STATUS   error_no,
					HPDF_STATUS   detail_no,
					void         *user_data	)
{
    printf("ERROR: error_no=%04X, detail_no=%u\n", (HPDF_UINT)error_no,(HPDF_UINT)detail_no);
    longjmp(env, 1);
}

int main (int argc, char **argv)
{
    HPDF_Doc  pdf;
    HPDF_Font font;
    HPDF_Page page;
	HPDF_Image iLogo;
    char fname[256];
    char tBuff[256];
	int tHidx = 0;
	int tDidx = 0;
	int tDidy = 0;

    float pageWidth=0;
    float pageHeight=0;
    double iLogoW=0.0;
    double iLogoH=0.0;
    double xLogo=0.0;
    double yLogo=0.0;
    double tBaseW=0.0;
    double tBaseH=0.0;
	
    strcpy (fname, argv[0]);
    strcat (fname, ".pdf");

    pdf = HPDF_New(error_handler,NULL);
    if(!pdf)
	{
        printf ("error: cannot create PdfDoc object\n");
        return 1;
    }

    if(setjmp(env))
	{
        HPDF_Free(pdf);
        return 1;
    }

    /* create default-font */
    font = HPDF_GetFont(pdf, "Helvetica", NULL);

    /* add a new page object. */
    page = HPDF_AddPage(pdf);
	
	/* Set page size */
	HPDF_Page_SetSize(page,HPDF_PAGE_SIZE_A4,HPDF_PAGE_PORTRAIT);

    pageWidth = HPDF_Page_GetWidth(page);
    pageHeight = HPDF_Page_GetHeight(page);

    /* print the lines of the page. */
    HPDF_Page_SetLineWidth(page, 1);
    HPDF_Page_Rectangle( page, PDF_BARDER_WIDTH, PDF_BARDER_HEIGHT, 
						(pageWidth - (PDF_BARDER_WIDTH + PDF_BARDER_HEIGHT)), 
						(pageHeight - (PDF_BARDER_WIDTH + PDF_BARDER_HEIGHT)));
    HPDF_Page_Stroke(page);

	/* Header Line */
    HPDF_Page_SetLineWidth (page, 3);
    HPDF_Page_SetRGBStroke(page, 0.71, 0.45, 0);
    HPDF_Page_SetLineCap (page, HPDF_ROUND_END);
	
	HPDF_Page_MoveTo(page, 40, (pageHeight - 80));
    HPDF_Page_LineTo(page, (40 + (pageWidth - 80)), (pageHeight - 80));
    HPDF_Page_Stroke(page);

	/* Footer Line */
	HPDF_Page_MoveTo(page, 40, 60);
    HPDF_Page_LineTo(page, (40 + (pageWidth - 80)), 60);
    HPDF_Page_Stroke(page);

	/* Header Heading */
	HPDF_Page_SetRGBFill(page, 0.30, 0.0, 0.0);
	HPDF_Page_SetFontAndSize(page, font, 18);
	HPDF_Page_BeginText(page);
    HPDF_Page_MoveTextPos(page, ((pageWidth/2)+115), (pageHeight - 75));
    HPDF_Page_ShowText(page, "MODBUS to PDF");
    HPDF_Page_EndText(page);

	/* Footer Heading */
	HPDF_Page_SetRGBFill(page, 0.30, 0.0, 0.0);
	HPDF_Page_SetFontAndSize(page, font, 10);
	HPDF_Page_BeginText(page);
    HPDF_Page_MoveTextPos(page, 40, 48);
    HPDF_Page_ShowText(page, "Copyright @ 2020");
    HPDF_Page_EndText(page);

	HPDF_Page_BeginText(page);
    HPDF_Page_MoveTextPos(page, ((pageWidth/2)+110), 48);
    HPDF_Page_ShowText(page, "Generated : 02/02/2020 20:21:57");
    HPDF_Page_EndText(page);

	/* Set Logo */
	iLogo = HPDF_LoadPngImageFromFile (pdf, "logo.png");
	iLogoW = HPDF_Image_GetWidth(iLogo);
    iLogoH = HPDF_Image_GetHeight(iLogo);
    xLogo = 40;
    yLogo = (pageHeight - 78);
    HPDF_Page_DrawImage(page, iLogo, xLogo, yLogo, iLogoW, iLogoH);

	/* Modbus Data as Table Format */
	/* Set Table Headings */
	font = HPDF_GetFont(pdf, "Helvetica-Bold", NULL);
	HPDF_Page_SetFontAndSize(page, font, 14);
	tBaseW = 40;
	for(tHidx=0; (tableHeadings[tHidx] != NULL); tHidx++)
	{
		HPDF_Page_BeginText(page);
		HPDF_Page_MoveTextPos(page, tBaseW, (pageHeight - 96)); //(30 + 50 + 14 + 2)
		HPDF_Page_ShowText(page, tableHeadings[tHidx]);
		HPDF_Page_EndText(page);
		tBaseW += ((strlen(tableHeadings[tHidx]) + 4) * 8 );
	}

	/* Table Header Line */
    HPDF_Page_SetLineWidth (page, 1.5);
    HPDF_Page_SetLineCap (page, HPDF_ROUND_END);
	HPDF_Page_MoveTo(page, 40, (pageHeight - 100));
    HPDF_Page_LineTo(page, (40 + (pageWidth - 80)), (pageHeight - 100)); // //(30 + 50 + 14 + 2 + 4)
    HPDF_Page_Stroke(page);

	/* Sample Data */
	font = HPDF_GetFont(pdf, "Helvetica", NULL);
	HPDF_Page_SetFontAndSize(page, font, 12);
	tBaseH = 114; //(30 + 50 + 14 + 2 + 4 + 12 + 2)
	for(tDidy=1; tDidy<= 48; tDidy++)
	{
		tBaseW = 40;
		for(tDidx=0; (tableHeadings[tDidx] != NULL); tDidx++)
		{
			HPDF_Page_BeginText(page);
			HPDF_Page_MoveTextPos( page, tBaseW, (pageHeight - tBaseH) );
			memset(tBuff,0,sizeof(tBuff));
			
			switch(tDidx)
			{
				case 0:
					sprintf(tBuff,"%d",tDidy);
				break;
				case 1:
					sprintf(tBuff,"Param_%d",tDidy);
				break;
				case 2:
					sprintf(tBuff,"Val_%d",tDidy);
				break;
				case 3:
					sprintf(tBuff,"Uni_%d",tDidy);
				break;
				default:
					sprintf(tBuff,"%s_%d",tableHeadings[tDidx],tDidy);
				break;
			}
			HPDF_Page_ShowText(page, tBuff);
			HPDF_Page_EndText(page);
			tBaseW += ((strlen(tableHeadings[tDidx]) + 4) * 8 );
		}
		tBaseH += 14; // 12 + 2
	}

    /* save the document to a file */
    HPDF_SaveToFile(pdf, fname);

    /* clean up */
    HPDF_Free(pdf);

    return 0;
}

