//! @file
//! @author cppclic.xsl - CLI library 2.7 (Alexis Royer, http://alexis.royer.free.fr/CLI/)
//! @warning File auto-generated by 'cppclic.xsl' - Do not edit!

// ----- Pre-compiled headers -----
#include "cli/pch.h"

// ----- Extra cpp (option='head') -----

// ----- Includes -----
#include "cli/common.h"

// ----- Extra cpp (option='include') -----

// ----- Extra cpp (option='types') -----

// ----- Extra cpp (option='vars') -----

// ----- Cli class definition -----
class myCLI : public cli::Cli {
    // ----- Sub-menus -----

    // ----- Owner CLI -----
    private: myCLI* m_pcliOwnerCli;
    // ----- Menus -----
    private: myCLI* m_pcli_cli_id4487326;
    // ----- Node members -----
    // ----- Extra cpp (option='members') -----

    // ----- Constructor -----
    public: myCLI(void) :
        cli::Cli("myCLI", cli::Help())
    {
        Populate();
        // ----- Extra cpp (option='constructor') -----
    }

    // ----- Destructor -----
    public: virtual ~myCLI(void) {
    }

    // ----- Populate -----
    public: void Populate(void) {
        // CLI reference
        m_pcliOwnerCli = dynamic_cast<myCLI*>(const_cast<cli::Cli*>(& GetCli()));
        // Create menus and populate
        m_pcliOwnerCli->m_pcli_cli_id4487326 = this;
        // Local nodes
    }

    // ----- Menu execution -----
    public: virtual const bool Execute(const cli::CommandLine& CLI_CmdLine) const {
        {
            static const cli::TraceClass CLI_EXECUTION("CLI_EXECUTION", cli::Help().AddHelp(cli::Help::LANG_EN, "CLI Execution traces").AddHelp(cli::Help::LANG_FR, "Traces d'ex�cution du CLI"));
            cli::CommandLineIterator cli_Elements(CLI_CmdLine);
            // myCLI>
            m_pcli_cli_id4487326_top_lbl: ;
            {
                if (! cli_Elements.StepIt()) return false;
                cli::GetTraces().Trace(CLI_EXECUTION) << "context = \"myCLI>\", " << "word = " << (dynamic_cast<const cli::Endl*>(*cli_Elements) ? "<CR>" : (const char*) (*cli_Elements)->GetKeyword()) << cli::endl;
                return false;
            }
            m_pcli_cli_id4487326_end_lbl: ;
        }
        return false;
    }

    public: virtual const bool OnError(const cli::ResourceString& location, const cli::ResourceString& message) const {
        return Cli::OnError(location, message);
    }

    public: virtual void OnExit(void) const {
    }

    public: virtual const cli::tk::String OnPrompt(void) const {
        return Menu::OnPrompt();
    }

};

// ----- Node creation -----

// ----- Extra cpp (option='body') -----

// ----- Extra cpp (option='tail') -----

