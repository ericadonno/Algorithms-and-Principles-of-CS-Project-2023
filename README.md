# 🍰 API Project: Pastry Automation System

## Description
This project was developed within the framework of the Algorithms and Principles of Informatics course, a requirement for the bachelor's degree in computer engineering at the Polytechnic University of Milan (Politecnico di Milano).

Serving as the final examination, the project focused on the automation of the inner workings of a simulated pastry.

The course was taught by Prof. Alessandro Barenghi during the 2023-2024 academic year.

Evaluation of the project was carried out automatically via a proprietary platform belonging to the DEIB (Department of Electronics, Information, and Bioengineering) at the Polytechnic University of Milan.

**Final Score: 30/30**

## Project Overview
This project is a discrete-time simulation of an industrial pastry shop's order, inventory, and logistics management system, developed using C. It models the entire process from customer order placement to final courier shipment, with a focus on efficient data structures to meet strict chronological and expiration-based priorities.

The simulation operates in discrete time steps , where the execution of a single command advances the time by one instant. The core challenge is maintaining strict order and inventory control under dynamic conditions, ensuring both timely fulfillment and minimal waste.

Key Simulation Mechanics
* Recipes and Ingredients: Recipes are defined by the required ingredients and quantities. The weight of a prepared pastry is the sum of its ingredient quantities.
Implementation Detail: Recipes are stored in a hash table for O(1) average time lookup, ensuring fast order validation.

* Ingredient Stock Management: The warehouse stores ingredient lots, each characterized by a quantity and an expiration time. Ingredient lots are managed via a **hash table** where each linked list is sorted by expiration time (earliest first). This design directly enforces the rule to draw from lots with the most proximate expiration in an efficient manner.
  
* Orders are placed in a waiting queue if full preparation is impossible due to insufficient stock. Waiting orders are managed in a **FIFO (First-In, First-Out) linked list** to ensure they are processed in chronological order of arrival.

* Refill and Clearance: Following an ingredient refill (rifornimento), the system immediately attempts to prepare any waiting orders, prioritizing them chronologically.

* Courier Logistics: A courier arrives periodically, selecting prepared orders based on a sequence of rules until their maximum weight capacity is reached.
  
* Orders are initially chosen in chronological order of arrival. The final loading process uses a **temporary sorted linked list** to prioritize selected orders onto the truck in decreasing order of total weight , using the order's arrival time to break ties.

## 🛠️ Tools Used

The following tools were essential for development, testing, and performance analysis:

* **GCC + Ubuntu**: Primary development environment and compiler.
* **Memcheck, AddressSanitizer**: Utilized for detecting memory errors such as leaks, illegal access, and buffer overflows.
* **Massif (Massif-visualizer)**: Used to profile and visualize the heap allocation over time.
* **Callgrind (Kcachegrind)**: Employed to control and analyze execution time, identifying performance bottlenecks.
