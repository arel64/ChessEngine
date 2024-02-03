#ifndef MACRO_COMMAND_HPP
#define MACRO_COMMAND_HPP
    #include "Engine.hpp"
    #include <unordered_map>
    enum class UCICommandType
    {
        UCI ,
        Debug,
        IsReady,
        SetOption,
        //Register,
        UciNewGame,
        Position,
        Go,
        Stop,
        //PonderHit,
        Quit,
        Unknown,
        Ignore
    };
    class UCICommand
    {
        public:
            UCICommand(std::shared_ptr<std::vector<std::string>> arglist,std::shared_ptr<Engine> engine);
            std::shared_ptr<Engine> getEngine() const {return engine;}    
            virtual int execute() = 0;
        protected:
            std::shared_ptr<std::vector<std::string>> arglist;
            std::shared_ptr<Engine> engine;
    };

#endif