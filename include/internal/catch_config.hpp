/*
 *  Created by Phil on 08/11/2010.
 *  Copyright 2010 Two Blue Cubes Ltd. All rights reserved.
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef TWOBLUECUBES_CATCH_RUNNERCONFIG_HPP_INCLUDED
#define TWOBLUECUBES_CATCH_RUNNERCONFIG_HPP_INCLUDED

#include "catch_interfaces_reporter.h"
#include "catch_context.h"

#include <memory>
#include <vector>
#include <string>
#include <iostream>

namespace Catch {

    struct Include { enum WhichResults {
        FailedOnly, 
        SuccessfulResults
    }; };

    struct List{ enum What {
        None = 0,
        
        Reports = 1,
        Tests = 2,
        All = 3,
        
        WhatMask = 0xf,
        
        AsText = 0x10,
        AsXml = 0x11,
        
        AsMask = 0xf0
    }; };
    
    struct ConfigData {
        ConfigData()
        :   listSpec( List::None ),
            shouldDebugBreak( false ),
            includeWhichResults( Include::FailedOnly ),
            cutoff( -1 ),
            allowThrows( true )
        {}
        
        std::string reporter;
        std::string outputFilename;
        List::What listSpec;
        std::vector<std::string> testSpecs;
        bool shouldDebugBreak;
        std::string stream;
        Include::WhichResults includeWhichResults;
        std::string name;
        int cutoff;
        bool allowThrows;
    };
    
    
    class Config : public IConfig {
    private:
        Config( const Config& other );
        Config& operator = ( const Config& other );
    public:
                
        Config()
        :   m_streambuf( NULL ),
            m_os( std::cout.rdbuf() )
        {}
        
        Config( const ConfigData& data )
        :   m_data( data ),
            m_streambuf( NULL ),
            m_os( std::cout.rdbuf() )
        {}
        
        ~Config() {
            m_os.rdbuf( std::cout.rdbuf() );
            delete m_streambuf;
        }
        
        void setFilename( const std::string& filename ) {
            m_data.outputFilename = filename;
        }
        
        bool testsSpecified() const {
            return !m_data.testSpecs.empty();
        }

        const std::vector<std::string>& getTestSpecs() const {
            return m_data.testSpecs;
        }
        
        List::What getListSpec( void ) const {
            return m_data.listSpec;
        }

        const std::string& getFilename() const {
            return m_data.outputFilename ;
        }
        
        List::What listWhat() const {
            return static_cast<List::What>( m_data.listSpec & List::WhatMask );
        }        
        
        List::What listAs() const {
            return static_cast<List::What>( m_data.listSpec & List::AsMask );
        }

        std::string getName() const {
            return m_data.name;
        }
        
        bool shouldDebugBreak() const {
            return m_data.shouldDebugBreak;
        }

        virtual std::ostream& stream() const {
            return m_os;
        }
        
        void setStreamBuf( std::streambuf* buf ) {
            m_os.rdbuf( buf ? buf : std::cout.rdbuf() );
        }        

        void useStream( const std::string& streamName ) {
            std::streambuf* newBuf = Context::createStreamBuf( streamName );
            setStreamBuf( newBuf );
            delete m_streambuf;
            m_streambuf = newBuf;
        }
        
        virtual bool includeSuccessfulResults() const {
            return m_data.includeWhichResults == Include::SuccessfulResults;
        }
        
        int getCutoff() const {
            return m_data.cutoff;
        }
        
        virtual bool allowThrows() const {
            return m_data.allowThrows;
        }
        
        ConfigData& data() {
            return m_data;
        }
        
    private:
        ConfigData m_data;
        
        // !TBD Move these out of here
        std::streambuf* m_streambuf;
        mutable std::ostream m_os;
    };
        
    
} // end namespace Catch

#endif // TWOBLUECUBES_CATCH_RUNNERCONFIG_HPP_INCLUDED
