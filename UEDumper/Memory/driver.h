// ReSharper disable CppNonInlineFunctionDefinitionInHeaderFile
#pragma once

//add any other includes here your driver might use
#include <Windows.h>
#include <tlhelp32.h>

#include "Frontend/Windows/LogWindow.h"
/*
██████╗░██╗░░░░░███████╗░█████╗░░██████╗███████╗  ██████╗░███████╗░█████╗░██████╗░██╗
██╔══██╗██║░░░░░██╔════╝██╔══██╗██╔════╝██╔════╝  ██╔══██╗██╔════╝██╔══██╗██╔══██╗██║
██████╔╝██║░░░░░█████╗░░███████║╚█████╗░█████╗░░  ██████╔╝█████╗░░███████║██║░░██║██║
██╔═══╝░██║░░░░░██╔══╝░░██╔══██║░╚═══██╗██╔══╝░░  ██╔══██╗██╔══╝░░██╔══██║██║░░██║╚═╝
██║░░░░░███████╗███████╗██║░░██║██████╔╝███████╗  ██║░░██║███████╗██║░░██║██████╔╝██╗
╚═╝░░░░░╚══════╝╚══════╝╚═╝░░╚═╝╚═════╝░╚══════╝  ╚═╝░░╚═╝╚══════╝╚═╝░░╚═╝╚═════╝░╚═╝
*/


/// here should be your driver functions
///	DO NOT call any of those functions from any class,
///	they should only get called from the memory class (memory.cpp and memory.h)
/// DO NOT include this file in any other file, you might get linker errors!
/// ANY CHANGES you do to the params in functions, make sure you also edit the memory.cpp and memory.h file!

#include <codecvt> // for std::wstring_convert
#include "MemStream/FPGA.h"
#include "MemStream/Process.h"
#pragma comment(lib, "memstream")
//global variables here
HANDLE procHandle = nullptr;
memstream::FPGA* fpga = nullptr;
memstream::Process* gProc = nullptr;

//in case you need to initialize anything BEFORE your com works, you can do this in here.
//this function IS NOT DESIGNED to already take the process name as input or anything related to the target process
//use the function "load" below which will contain data about the process name
inline void init()
{
    if(fpga == nullptr)
        fpga = new memstream::FPGA();
}

uint64_t _getBaseAddress(const wchar_t* processName, int& pid);

void attachToProcess(const int& pid);

/**
 * \brief use this function to initialize the target process
 * \param processName process name as input
 * \param baseAddress base address of the process gets returned
 * \param processID process id of the process gets returned
 */
inline void loadData(std::string& processName, uint64_t& baseAddress, int& processID)
{
    if(fpga == nullptr) {
        windows::LogWindow::Log(windows::LogWindow::logLevels::LOGLEVEL_ERROR, "MEMORY", "DMA not initialized");
        return;
    }
    const auto name = std::wstring(processName.begin(), processName.end());
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::string name_string = converter.to_bytes(name);

    if(gProc == nullptr)
        gProc = new memstream::Process(fpga, name_string);

    baseAddress = gProc->GetModuleBase(name_string);
    processID = gProc->getPid();
}

/**
 * \brief read function (replace with your read logic)
 * \param address memory address to read from
 * \param buffer memory address to write to
 * \param size size of memory to read (expects the buffer/address to have this size too)
 */
inline void _read(const void* address, void* buffer, const DWORD64 size)
{
    if(fpga == nullptr || gProc == nullptr) {
        windows::LogWindow::Log(windows::LogWindow::logLevels::LOGLEVEL_ERROR, "MEMORY", "DMA not initialized");
        return;
    }
    size_t bytes_read = 0;
    if(!gProc->Read((uint64_t)address, (uint8_t*)buffer, size)) {
        windows::LogWindow::Log(windows::LogWindow::logLevels::LOGLEVEL_ERROR, "MEMORY", "Error Reading address %llX", address);
    }
}


/**
 * \brief write function (replace with your write logic)
 * \param address memory address to write to
 * \param buffer memory address to write from
 * \param size size of memory to write (expects the buffer/address to have this size too)
 */
inline void _write(void* address, const void* buffer, const DWORD64 size)
{
    if(fpga == nullptr || gProc == nullptr) {
        windows::LogWindow::Log(windows::LogWindow::logLevels::LOGLEVEL_ERROR, "MEMORY", "DMA not initialized");
        return;
    }
    gProc->Write((uint64_t)address, (uint8_t*)buffer, size);
}


/**
 * \brief gets the process base address. If you adjust the params, make sure to change them in memory.cpp too
 * \param processName the name of the process
 * \param pid returns the process id
 * \return process base address
 */
uint64_t _getBaseAddress(const wchar_t* processName, int& pid)
{
    uint64_t baseAddress = 0;
    if(fpga == nullptr) {
        windows::LogWindow::Log(windows::LogWindow::logLevels::LOGLEVEL_ERROR, "MEMORY", "DMA not initialized");
        return 0;
    }

    if(gProc == nullptr) {
        gProc = new memstream::Process(fpga, pid);
        baseAddress = gProc->GetModuleBase(gProc->getName());
    }

    return baseAddress;
}

/**
 * \brief this function might not be needed for your driver, this just attaches to the process
 * \param pid process id of the target process
 */
void attachToProcess(const int& pid)
{
    procHandle = nullptr;
}