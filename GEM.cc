#undef G4MULTITHREADED
#define G4DEBUG_FIELD

//#undef G4VIS_USE
//#undef G4UI_USE

#include <cstdio>
#include <ctime>

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#include "G4UImanager.hh"
#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

#include "GEMPhysicsList.hh"
#include "GEMDetectorConstruction.hh"
#include "GEMPrimaryGeneratorAction.hh"
#include "GEMActionInitialization.hh"
#include "GEMVoxParallelWorld.hh"
#include "G4Navigator.hh"
#include "G4GeometryTolerance.hh"
#include "G4Navigator.hh"
#include "G4GeometryManager.hh"
#include "G4TransportationManager.hh"
#include "G4PropagatorInField.hh"
#include "G4PathFinder.hh"
#include "G4FPEDetection.hh"
#include "G4RunManagerKernel.hh"
#include "G4Threading.hh"
#include "G4WorkerRunManager.hh"
#include "G4EmParameters.hh"
#include "G4VisManager.hh"

int main(int argc,char** argv)
{
    G4UIExecutive* ui = 0;
    if ( argc == 1 ) {
        ui = new G4UIExecutive(argc, argv);
    }


    // Set the custom seed for the random engine
    G4Random::setTheEngine(new CLHEP::RanecuEngine);
    G4long seed = time(NULL);
    G4Random::setTheSeed(seed);

#ifdef G4MULTITHREADED
    G4MTRunManager* runManager = new G4MTRunManager;
    runManager->SetNumberOfThreads(6);
#else
    G4RunManager* runManager = new G4RunManager;
#endif

    G4GeometryManager::GetInstance()->SetWorldMaximumExtent(4.0*m);

    GEMDetectorConstruction* massWorld = new GEMDetectorConstruction;
    massWorld->RegisterParallelWorld(new GEMVoxParallelWorld("GEMVoxParallelWorld"));
    runManager->SetUserInitialization(massWorld);

    G4VModularPhysicsList* physicsList = new GEMPhysicsList;
    physicsList->SetVerboseLevel(0);
    runManager->SetUserInitialization(physicsList);

    runManager->SetUserInitialization(new GEMActionInitialization);
    runManager->Initialize();

    G4Navigator* navigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
    navigator->SetPushVerbosity(false);

    // Initialize visualization
    //
    G4VisManager* visManager = new G4VisExecutive;
#ifdef G4VIS_USE_OPENGL
  G4VGraphicsSystem* ogl  = 0;
  G4VGraphicsSystem* ogli = 0;
  G4VGraphicsSystem* ogls = 0;
  G4VGraphicsSystem* ogl_fallback = 0;
  G4VGraphicsSystem* ogli_fallback = 0;
  G4VGraphicsSystem* ogls_fallback = 0;
#ifdef G4VIS_USE_OPENGLQT
  ogl  = new G4OpenGLStoredQt;
  ogli = new G4OpenGLImmediateQt;
  ogls = new G4OpenGLStoredQt;
#if defined G4VIS_USE_OPENGLXM
  ogl_fallback  = new G4OpenGLStoredXm;
  ogli_fallback = new G4OpenGLImmediateXm;
  ogls_fallback = new G4OpenGLStoredXm;
#elif defined G4VIS_USE_OPENGLX
  ogl_fallback  = new G4OpenGLStoredX;
  ogli_fallback = new G4OpenGLImmediateX;
  ogls_fallback = new G4OpenGLStoredX;
#elif defined G4VIS_USE_OPENGLWIN32
  ogl_fallback  = new G4OpenGLStoredWin32;
  ogli_fallback = new G4OpenGLImmediateWin32;
  ogls_fallback = new G4OpenGLStoredWin32;
#endif
#elif defined G4VIS_USE_OPENGLWT
  ogl  = new G4OpenGLImmediateWt;
  ogli = new G4OpenGLImmediateWt;
  ogls = new G4OpenGLImmediateWt;
#elif defined G4VIS_USE_OPENGLXM
  ogl  = new G4OpenGLStoredXm;
  ogli = new G4OpenGLImmediateXm;
  ogls = new G4OpenGLStoredXm;
#elif defined G4VIS_USE_OPENGLWIN32
  ogl  = new G4OpenGLStoredWin32;
  ogli = new G4OpenGLImmediateWin32;
  ogls = new G4OpenGLStoredWin32;
#elif defined G4VIS_USE_OPENGLX
  ogl  = new G4OpenGLStoredX;
  ogli = new G4OpenGLImmediateX;
  ogls = new G4OpenGLStoredX;
#endif
  if (ogl) {
    ogl->SetNickname("OGL");
    visManager->RegisterGraphicsSystem (ogl);
  }
  if (ogli) {
    ogli->SetNickname("OGLI");
    visManager->RegisterGraphicsSystem (ogli);
  }
  if (ogls) {
    ogls->SetNickname("OGLS");
    visManager->RegisterGraphicsSystem (ogls);
  }
  if (ogl_fallback) {
    ogl_fallback->SetNickname("OGL_FALLBACK");
    visManager->RegisterGraphicsSystem (ogl_fallback);
  }
  if (ogli_fallback) {
    ogli_fallback->SetNickname("OGLI_FALLBACK");
    visManager->RegisterGraphicsSystem (ogli_fallback);
  }
  if (ogls_fallback) {
    ogls_fallback->SetNickname("OGLS_FALLBACK");
    visManager->RegisterGraphicsSystem (ogls_fallback);
  }
#endif
    // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
    // G4VisManager* visManager = new G4VisExecutive("Quiet");
    visManager->Initialize();

    // Get the pointer to the User Interface manager
    G4UImanager* UImanager = G4UImanager::GetUIpointer();

    // Process macro or start UI session
    //
    if ( ! ui ) {
        // batch mode
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command+fileName);
    }
    else {
        // interactive mode
        UImanager->ApplyCommand("/control/execute init_vis.mac");
        ui->SessionStart();
        delete ui;
    }

    // Job termination
    // Free the store: user actions, physics_list and detector_description are
    // owned and deleted by the run manager, so they should not be deleted
    // in the main() program !

    delete visManager;
    delete runManager;
}
