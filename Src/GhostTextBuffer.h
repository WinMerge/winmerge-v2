/**
 * @file  GhostTextBuffer.h
 *
 * @brief Declaration of CGhostTextBuffer (subclasses CCrystalTextBuffer to handle ghost lines)
 */
#pragma once

#include <vector>
#include "ccrystaltextbuffer.h"


/////////////////////////////////////////////////////////////////////////////

/**
 * We use the current ccrystalEditor flags 
 *
 * This flag must be cleared and set in GhostTextBuffer.cpp 
 * and MergeDoc.cpp (Rescan) only.
 *
 * GetLineColors (in MergeEditView) reads it to choose the line color.
 */
enum GHOST_LINEFLAGS
{
	LF_GHOST = 0x00400000L, /**< Ghost line. */
};

/////////////////////////////////////////////////////////////////////////////
// CCrystalTextBuffer command target

/**
 * @brief A class handling ghost lines.
 * Features offered with this class : 
 * <ul>
 *  <li> apparent/real line conversion 
 *  <li> insertText/deleteText working with ghost lines 
 *  <li> AddUndoRecord/Undo/Redo working with ghost lines 
 *  <li> insertGhostLine function 
 * </ul>
 */
class EDITPADC_CLASS CGhostTextBuffer : public CCrystalTextBuffer
{
public:
	DECLARE_DYNCREATE (CGhostTextBuffer)

private:
	/**
	 * @brief A struct mapping real lines and apparent (screen) lines.
	 * This struct maps lines between real lines and apparent (screen) lines.
	 * The mapping records for each text block an apparent line and matching
	 * real line.
	 */
	struct RealityBlock
	{
		int nStartReal; /**< Start line of real block. */
		int nStartApparent; /**< Start line of apparent block. */
		int nCount; /**< Lines in the block. */
	};
	std::vector<RealityBlock> m_RealityBlocks; /**< Mapping of real and apparent lines. */

	// Operations
private:
	bool InternalInsertGhostLine (CCrystalTextView * pSource, int nLine);
	bool InternalDeleteGhostLine (CCrystalTextView * pSource, int nLine, int nCount);
public :
	// Construction/destruction code
	CGhostTextBuffer ();

	/** 
	This should work in base code as ghost lines are real empty lines
	but maybe it doesn't (if there is an assert to check there is an EOL,
	or if it adds the default EOL)
	*/
	virtual void GetTextWithoutEmptys (int nStartLine, int nStartChar,
			int nEndLine, int nEndChar, CString &text,
			CRLFSTYLE nCrlfStyle =CRLF_STYLE_AUTOMATIC,
			bool bExcludeInvisibleLines = true);


	// Text modification functions
	virtual bool InsertText (CCrystalTextView * pSource, int nLine, int nPos,
		LPCTSTR pszText, int cchText, int &nEndLine, int &nEndChar,
		int nAction = CE_ACTION_UNKNOWN, bool bHistory =true);
	virtual bool DeleteText2 (CCrystalTextView * pSource, int nStartLine,
		int nStartPos, int nEndLine, int nEndPos,
		int nAction = CE_ACTION_UNKNOWN, bool bHistory =true);
	bool InsertGhostLine (CCrystalTextView * pSource, int nLine);

	virtual void AddUndoRecord (bool bInsert, const CPoint & ptStartPos, const CPoint & ptEndPos,
	                            LPCTSTR pszText, int cchText, int nActionType = CE_ACTION_UNKNOWN, CDWordArray *paSavedRevisionNumbers = NULL);
	virtual UndoRecord GetUndoRecord(int nUndoPos) const;

	virtual CDWordArray *CopyRevisionNumbers(int nStartLine, int nEndLine) const;
	virtual void RestoreRevisionNumbers(int nStartLine, CDWordArray *paSavedRevisionNumbers);

public:
	//@{
	/**
	 * @name Real/apparent line number conversion functions.
	 * These functions convert line numbers between file line numbers
	 * (real line numbers) and screen line numbers (apparent line numbers).
	 *
	 * This mapping is needed to handle ghost lines (ones with no text or
	 * EOL chars) which WinMerge uses for left-only or right-only lines.
	*/
	int ApparentLastRealLine() const;
	int ComputeRealLine(int nApparentLine) const;
	int ComputeApparentLine(int nRealLine) const;
	/** richer position information   yApparent = apparent(yReal) - yGhost */
	int ComputeRealLineAndGhostAdjustment(int nApparentLine, int& decToReal) const;
	/** richer position information   yApparent = apparent(yReal) - yGhost */
	int ComputeApparentLine(int nRealLine, int decToReal) const;
	//@}

	/** for loading file */
	void FinishLoading();
	/** for saving file */ 
	void RemoveAllGhostLines();


private:
	void RecomputeRealityMapping();
	/** For debugging purpose */
	void checkFlagsFromReality(bool bFlag) const;

protected:
	virtual void OnNotifyLineHasBeenEdited(int nLine);


protected:
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCrystalTextBuffer)
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CCrystalTextBuffer)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP ()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.