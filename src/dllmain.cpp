#include "common.hpp"

void HackThread(HMODULE instance)
{
    std::filesystem::path base_dir = std::getenv("appdata");
    base_dir /= "MultiHack";
    std::unique_ptr<file_manager, std::default_delete<file_manager>> file_manager_instance;
    std::unique_ptr<thread_pool, std::default_delete<thread_pool>> thread_pool_instance;

#if _DEBUG
    console::open();
#endif

    try {
        file_manager_instance = std::make_unique<file_manager>(base_dir);
        thread_pool_instance = std::make_unique<thread_pool>();

		interfaces::Setup();
		gui::Setup();
		hooks::Setup();
		netvars::SetupNetvars();
		g.init(file_manager_instance->get_project_file("./settings.json"));
    }
    catch (const std::exception& error)
    {
        MessageBeep(MB_ICONERROR);
        MessageBox(
            0,
            error.what(),
            "hack error",
            MB_OK | MB_ICONEXCLAMATION
        );

        goto UNLOAD;
    }

    while (!GetAsyncKeyState(VK_END)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

UNLOAD:
#if _DEBUG
    console::close();
#endif
    g.attempt_save();
    g.destroy();
    hooks::Destroy();
    thread_pool_instance->destroy();
    thread_pool_instance.reset();
    file_manager_instance.reset();
    FreeLibraryAndExitThread(instance, 0);
}

BOOL APIENTRY DllMain(HMODULE instance, DWORD reason, LPVOID reserved)
{
    switch (reason)
    {
        case DLL_PROCESS_ATTACH:
        {
            DisableThreadLibraryCalls(instance);
            
            if (const auto thread = CreateThread(
                nullptr,
                0,
                reinterpret_cast<LPTHREAD_START_ROUTINE>(HackThread),
                instance,
                0,
                nullptr
            ))
                CloseHandle(thread);
        }

    }
    return TRUE;
}

