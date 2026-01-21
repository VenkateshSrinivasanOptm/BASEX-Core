# BASEX Core

## Overview
**BASEX Core** is a reusable, service-based base platform designed to act as the foundation for multiple products across domains such as automotive, infotainment, advanced clusters, EV chargers, and embedded gateways.

This project provides a **common runtime environment** where all core services are initialized and brought up automatically at system start, along with a minimal common UI to indicate system readiness.

---

## Key Concepts

### Service-Based Architecture
BASEX Core follows a modular, service-oriented architecture where each functionality runs as an independent service.  
Examples include (but are not limited to):

- Bluetooth Service  
- Wi-Fi Service  
- CAN Service  
- GSM Service  
- Database Service  
- FOTA Service  
- Download / Upload Services  
- HMI Service  
- RFID Service  

Each service is responsible for:
- Its own initialization
- Self bring-up during system start
- Remaining idle or active based on configuration

---

## Self Bring-Up Mechanism
On application start:
- All registered services are instantiated
- Each service performs its basic readiness logic
- The system reaches a **ready state** without requiring external triggers

This allows BASEX Core to function as a **plug-and-play base platform**.

---

## Customization & Extension
BASEX Core is intentionally **generic**.

Product teams are expected to:
- Enable or disable services based on product needs
- Extend existing services with product-specific logic
- Add new services without impacting the core framework
- Customize the UI and workflows as required

No product-specific behavior is hard-coded into BASEX Core.

---

## Common UI
The project includes a minimal, generic UI screen intended to:
- Indicate BASEX Core runtime status
- Act as a placeholder for product-specific UI layers
- Provide a consistent base across all derived products

---

## Intended Usage
BASEX Core is meant to be used as:
- A **base repository** for new products
- A **shared platform** across teams
- A starting point for:
  - Automotive infotainment systems
  - Advanced digital clusters
  - EV charging systems
  - Embedded and gateway devices

---

## Repository Philosophy
- Keep the core **clean and generic**
- Avoid product-specific dependencies
- Favor configuration over modification
- Ensure services remain loosely coupled

---

## Getting Started
1. Clone the repository
2. Review available services
3. Enable / customize services as per product requirements
4. Build and run the platform
5. Extend on top of BASEX Core

---

## License
Internal company platform – usage and distribution as per company policy.
