#include "framework.h"
#include "lnx_fileSet.h"
#include "WFileFind.h"


lnx_fileSet::lnx_fileSet(::ca::application * papp) :
   ca(papp)
{
}

lnx_fileSet::~lnx_fileSet()
{

}

void lnx_fileSet::add_search(stringa & stra, bool_array & baRecursive)
{
   m_straSearch.add(stra);
   m_baRecursive.add(baRecursive);
   refresh();
}

void lnx_fileSet::add_filter(stringa & stra)
{
   m_straFilter.add(stra);
   refresh();
}

int lnx_fileSet::get_file_count()
{
   return m_straFile.get_size();
}

void lnx_fileSet::file_at(int i, string & str)
{
   str = m_straFile[i];
}

int lnx_fileSet::find_first_file(const char * lpcsz)
{
   return m_straFile.find_first_ci(lpcsz);
}

void lnx_fileSet::clear_search()
{
  m_straSearch.remove_all();
}

void lnx_fileSet::clear_filter()
{
  m_straFilter.remove_all();
}

void lnx_fileSet::clear_file()
{
   m_straFile.remove_all();
}

void lnx_fileSet::refresh()
{
   clear_file();
   string strFilter;
   int i, j;
   FileFind filefind;
   string strFile;
   string str;

   for(i = 0; i < m_straSearch.get_size(); i++)
   {
      bool bRecursive = true;
      if(i < m_baRecursive.get_size())
         bRecursive = m_baRecursive[i];
      for(j = 0; j < m_straFilter.get_size(); j++)
      {
         string strFilter = m_straFilter.element_at(j);
         strFilter.trim("\\/");
         if(bRecursive)
         {
            System.dir().rls_pattern(m_straSearch.element_at(i), strFilter, &m_straFile);
         }
         else
         {
            System.dir().ls_pattern(m_straSearch.element_at(i), strFilter, &m_straFile);
         }
      }
   }
}
