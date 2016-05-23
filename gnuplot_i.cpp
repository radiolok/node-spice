////////////////////////////////////////////
//
// A C++ interface to gnuplot. 
//
// This is a direct translation from the C interface
// written by N. Devillard (which is available from
// http://ndevilla.free.fr/gnuplot/).
//
// As in the C interface this uses pipes and so wont
// run on a system that does'nt have POSIX pipe 
// support
//
// Rajarshi Guha
// <rajarshi@presidency.com>
//
// 07/03/03
//
////////////////////////////////////////////

#ifdef WIN32
#include <io.h>
#else
#include <stdarg.h>
#define PATH_MAXNAMESZ       4096
#endif
#include "gnuplot_i.hpp"

using namespace std;

/////////////////////////////
//
// A string tokenizer taken from
// http://www.sunsite.ualberta.ca/Documentation/Gnu/libstdc++-2.90.8/html/21_strings/stringtok_std_h.txt
//
/////////////////////////////
template <typename Container>
void
stringtok (Container &container, string const &in,
           const char * const delimiters = " \t\n")
{
    const string::size_type len = in.length();
          string::size_type i = 0;

    while ( i < len )
    {
        // eat leading whitespace
        i = in.find_first_not_of (delimiters, i);
        if (i == string::npos)
            return;   // nothing left but white space

        // find the end of the token
        string::size_type j = in.find_first_of (delimiters, i);

        // push token
        if (j == string::npos) {
            container.push_back (in.substr(i));
            return;
        } else
            container.push_back (in.substr(i, j-i));

        // set up for next loop
        i = j + 1;
    }
}

//
// Constructors
//
Gnuplot::Gnuplot(void)
{
	init();
    set_style("points");
}

Gnuplot::Gnuplot(const string &style)
{
	init();
	set_style(style);
}

Gnuplot::Gnuplot(
         const string &title,
         const string &style,
         const string &labelx,  const string &labely,
         vector<double> x, vector<double> y)
{
	init();
		
    if (x.size() == 0 || y.size() == 0)
        throw GnuplotException("vectors too small");

    if (style.empty())
        this->set_style("lines");
    else
        this->set_style(style);

    if (labelx.empty())
        this->set_xlabel("X");
    else
        this->set_xlabel(labelx);
    if (labely.empty())
        this->set_ylabel("Y");
    else
        this->set_ylabel(labely);
    
    this->plot_xy(x,y,title);

    cout << "Press enter to continue" << endl;
    while (getchar() != '\n'){}
}

Gnuplot::Gnuplot(
         const string &title,
         const string &style,
         const string &labelx,  const string &labely,
         vector<double> x)
{
	init();

    if (x.size() == 0)
        throw GnuplotException("vector too small");
    if (!this->gnucmd)
        throw GnuplotException("Could'nt open connection to gnuplot");

    if (style.empty())
        this->set_style("lines");
    else
        this->set_style(style);

    if (labelx.empty())
        this->set_xlabel("X");
    else
        this->set_xlabel(labelx);
    if (labely.empty())
        this->set_ylabel("Y");
    else
        this->set_ylabel(labely);
    
    this->plot_x(x,title);

    cout << "Press enter to continue" << endl;
    while (getchar() != '\n'){}
}

//
// Destructor
// 
Gnuplot::~Gnuplot()
{
    if ((this->to_delete).size() > 0)
    {
        for (unsigned int i = 0; i < this->to_delete.size(); i++)
            remove(this->to_delete[i].c_str());
    }
#ifdef WIN32
    if (_pclose(this->gnucmd) == -1)
#else
    if (pclose(this->gnucmd) == -1)
#endif
        cerr << "Problem closing communication to gnuplot" << endl;
    return;
}

bool Gnuplot::is_valid(void)
{
    return(this->valid);
}

bool Gnuplot::get_program_path(const string& pname)
{
    list<string> ls;
    char *path;

    path = getenv("PATH");
    if (!path)
      {
        cerr << "Path is not set" << endl;
        return false;
      }
    else
      {
#ifdef WIN32
        stringtok(ls,path,";");
#else
        stringtok(ls,path,":");
#endif
        for (list<string>::const_iterator i = ls.begin();
                i != ls.end(); ++i)
          {
            string tmp = (*i) + "/" + pname;
#ifdef WIN32
            if (_access(tmp.c_str(),0) == 0)
#else
            if (access(tmp.c_str(),X_OK) == 0)
#endif
                return true;
          }
      }
    return false;
}

void Gnuplot::reset_plot(void)
{       
    if (this->to_delete.size() > 0)
    {
        for (unsigned int i = 0; i < this->to_delete.size(); i++)
            remove(this->to_delete[i].c_str());
    }
    this->nplots = 0;
    return;
}

void Gnuplot::set_style(const string &stylestr)
{
    if (stylestr != "lines" &&
        stylestr != "points" &&
        stylestr != "linespoints" &&
        stylestr != "impulses" &&
        stylestr != "dots" &&
        stylestr != "steps" &&
        stylestr != "errorbars" &&
        stylestr != "boxes" &&
        stylestr != "boxerrorbars")
        this->pstyle = string("points");
    else
        this->pstyle = stylestr;
}

void Gnuplot::cmd(const string &cmdstr, ...)
{
    va_list ap;
    char local_cmd[GP_CMD_SIZE];

#ifdef WIN32
    va_start(ap, cmdstr);
#else
    va_start(ap, cmdstr.c_str());
#endif
    vsprintf(local_cmd, cmdstr.c_str(), ap);
    va_end(ap);
    strcat(local_cmd,"\n");
    fputs(local_cmd,this->gnucmd);
    fflush(this->gnucmd);
    return;
}

void Gnuplot::set_ylabel(const string &label)
{
    ostringstream cmdstr;

    cmdstr << "set ylabel \"" << label << "\"";
    this->cmd(cmdstr.str());

    return;
}

void Gnuplot::set_yrange(double ymin, double ymax)
{
    ostringstream cmdstr;

    cmdstr << "set yrange [ " << ymin << ":" << ymax << "]";
    this->cmd(cmdstr.str());

    return;
}

void Gnuplot::set_xrange(double xmin, double xmax)
{
    ostringstream cmdstr;

    cmdstr << "set yrange [ " << xmin << ":" << xmax << "]";
    this->cmd(cmdstr.str());

    return;
}

void Gnuplot::savePNG(const string &width,
						const string &height,
						const string &font,
						bool color,
						const string &filename)
{
    ostringstream cmdstr;

    cmdstr << "set term png size " << width << ", " << height << " enhanced font \"" << font << "\"";
    this->cmd(cmdstr.str());

    cmdstr.str("");
    cmdstr << "set output  \'" << filename << ".png\'";
    this->cmd(cmdstr.str());

    cmdstr.str("");
    cmdstr << "replot" ;
    this->cmd(cmdstr.str());

    return;
}

void Gnuplot::saveSVG(const string &width,
	const string &height,
	const string &font,
	bool color,
	const string &filename)
{
	ostringstream cmdstr;

	cmdstr << "set term svg size " << width << ", " << height << " enhanced font \"" << font << "\"";
	this->cmd(cmdstr.str());

	cmdstr.str("");
	cmdstr << "set output  \'" << filename << ".svg\'";
	this->cmd(cmdstr.str());

	cmdstr.str("");
	cmdstr << "replot";
	this->cmd(cmdstr.str());

	return;
}

void Gnuplot::set_xlabel(const string &label)
{
    ostringstream cmdstr;

    cmdstr << "set xlabel \"" << label << "\"";
    this->cmd(cmdstr.str());

    return;
}

void Gnuplot::show_grid(void){
    ostringstream cmdstr;

   // cmdstr <<
    cmdstr << "set grid";
    this->cmd(cmdstr.str());

    return;
}

// 
// Plots a linear equation (where you supply the
// slope and intercept)
//
void Gnuplot::plot_slope(double a, double b, const string &title)
{
    ostringstream stitle;
    ostringstream cmdstr;

    if (title.empty())
        stitle << "no title";
    else
        stitle << title;

    if (this->nplots > 0)
        cmdstr << "replot " << a << " * x + " << b << " title \"" << stitle.str() << "\" with " << pstyle;
    else
        cmdstr << "plot " << a << " * x + " << b << " title \"" << stitle.str() << "\" with " << pstyle;
    this->cmd(cmdstr.str());
    this->nplots++;
    return;
}

//
// Plot an equation which is supplied as a string
// 
void Gnuplot::plot_equation(const string &equation, const string &title)
{
    string titlestr, plotstr;
    ostringstream cmdstr;

    if (title.empty())
        titlestr = "no title";
    else
        titlestr = title;

    if (this->nplots > 0)
        plotstr = "replot";
    else
        plotstr = "plot";

    cmdstr << plotstr << " " << equation << " " << "title \"" << titlestr << "\" with " << this->pstyle;
    this->cmd(cmdstr.str());
    this->nplots++;

    return;
}

void Gnuplot::plot_x(vector<double> d, const string &title)
{
    //ofstream tmp;
    ostringstream cmdstr;
#ifdef WIN32
    char name[] = "gnuplotiXXXXXX";
#else
    char name[] = "/tmp/gnuplotiXXXXXX";
#endif

    if (this->to_delete.size() == GP_MAX_TMP_FILES - 1)
      {
        cerr << "Maximum number of temporary files reached (" << GP_MAX_TMP_FILES << "): cannot open more files" << endl;
        return;
      }

    if (d.size() == 0){
    	cout << "size of array is zero"<< endl;
     	return;
    }
    //
    //open temporary files for output
#ifdef WIN32
    if (_mktemp(name) == NULL)
#else
    if (mkstemp(name) == -1)
#endif
      {
        cerr << "Cannot create temporary file: exiting plot" << endl;
        return;
      }

	FILE *f = fopen(name, "w");
	if (f == NULL)
	{
		printf("Cannot create temorary file: exiting plot!\n");
		exit(1);
	}
    //
    // Save the temporary filename
    // 
	if (!saveraw){
		this->to_delete.push_back(name);
	}
    //
    // write the data to file
    //
	for (unsigned int i = 0; i < d.size(); i++)
		fprintf(f, "%f\n", d[i]);
     //   tmp << d[i] << endl;
    //tmp.flush();    
    //tmp.close();
	fclose(f);

    //
    // command to be sent to gnuplot
    //
    if (this->nplots > 0)
        cmdstr << "replot ";
    else cmdstr << "plot ";
    if (title.empty())
        cmdstr << "\"" << name << "\" with " << this->pstyle;
    else
        cmdstr << "\"" << name << "\" title \"" << title << "\" with " << this->pstyle;

    //
    // Do the actual plot
    //
    this->cmd(cmdstr.str());
    //this->show_grid();
    this->nplots++;

    return;
}
void Gnuplot::SaveRawFile(bool _state){
	saveraw = _state;
}

    
void Gnuplot::plot_xy(vector<double> x, vector<double> y, const string &title)
{
    //ofstream tmp;
    ostringstream cmdstr;
#ifdef WIN32
    char name[] = "gnuplotiXXXXXX";
#else
    char name[] = "/tmp/gnuplotiXXXXXX";
#endif
    // should raise an exception
    if (x.size() != y.size()){
    	cout << "invalid array data size(x) is not equal to size(y)"<< endl;
     	return;
    }

    if (x.size() == 0){
    	cout << "size of array is zero"<< endl;
     	return;
    }

    if ((this->to_delete).size() == GP_MAX_TMP_FILES - 1)
      {
        cerr << "Maximum number of temporary files reached (" << GP_MAX_TMP_FILES << "): cannot open more files" << endl;
        return;
      }
    //
    //open temporary files for output
    //
#ifdef WIN32
    if (_mktemp(name) == NULL)
#else
    if (mkstemp(name) == -1)
#endif
      {
        cerr << "Cannot create temporary file: exiting plot" << endl;
        return;
      }
	FILE *f = fopen(name, "w");
	if (f == NULL)
	{
		printf("Cannot create temorary file: exiting plot!\n");
		exit(1);
	}
    //
    // Save the temporary filename
    // 
	if (!saveraw){
		this->to_delete.push_back(name);
	}

    //
    // write the data to file
    //
    for (unsigned int i = 0; i < x.size(); i++)
    {
    	//tmp << x[i] << " " << y[i] << endl;
		fprintf(f, "%f %f\n", x[i], y[i]);
    }
	fclose(f);

    //
    // command to be sent to gnuplot
    //
    if (this->nplots > 0)
        cmdstr << "replot ";
    else cmdstr << "plot ";
    if (title.empty())
        cmdstr << "\"" << name << "\" with " << this->pstyle;
    else
        cmdstr << "\"" << name << "\" title \"" << title << "\" with " << this->pstyle;
    //
    // Do the actual plot
    //
    cout <<cmdstr.str() << endl;
    this->cmd(cmdstr.str());
    //this->show_grid();
    this->nplots++;
    return;
}


void Gnuplot::init()
{
#ifdef WIN32
    m_sGNUPlotFileName = "gnuplot.exe";
#else
    m_sGNUPlotFileName = "gnuplot";
#endif
    if (!this->get_program_path(m_sGNUPlotFileName))
    {
        this->valid = false;
        throw GnuplotException("Can't find gnuplot in your PATH");
    }
#ifdef WIN32
    this->gnucmd = _popen(m_sGNUPlotFileName.c_str(),"w");
#else
    this->gnucmd = popen(m_sGNUPlotFileName.c_str(),"w");
#endif

    if (!this->gnucmd)
    {
        this->valid = false;
        throw GnuplotException("Couldn't open connection to gnuplot");
    }
    this->nplots = 0;
    this->valid = true;

	this->saveraw = false;
}
