# Local Setup Instructions for Planto Project

## Step 1: Modify Package.json
Remove Replit-specific dependencies from the package.json file by deleting these lines:
```json
"@replit/vite-plugin-cartographer": "^0.1.2",
"@replit/vite-plugin-runtime-error-modal": "^0.0.3",
```

## Step 2: Modify vite.config.ts
1. Remove the import for the Replit runtime error overlay:
```typescript
// Remove this line
import runtimeErrorOverlay from "@replit/vite-plugin-runtime-error-modal";
```

2. Remove the Replit plugins from the plugins array:
```typescript
// Change this
plugins: [
  react(),
  runtimeErrorOverlay(),
  ...(process.env.NODE_ENV !== "production" &&
  process.env.REPL_ID !== undefined
    ? [
        await import("@replit/vite-plugin-cartographer").then((m) =>
          m.cartographer(),
        ),
      ]
    : []),
],

// To this
plugins: [
  react(),
],
```

3. Remove any REPL_ID references in the file.

## Step 3: Edit client/index.html
Remove the Replit development banner script from the HTML file:
```html
<!-- Remove this line -->
<script type="text/javascript" src="https://replit.com/public/js/replit-dev-banner.js"></script>
```

## Step 4: Install Dependencies and Run
1. Open a terminal in the project directory
2. Run `npm install` to install dependencies
3. Run `npm run dev` to start the development server

## Step 5: Troubleshooting Common Issues
If you encounter errors about missing modules or unresolved imports:

1. **Missing dependencies**: Install additional dependencies as needed
   ```
   npm install <package-name>
   ```

2. **TypeScript errors**: Make sure TypeScript is installed
   ```
   npm install typescript @types/node @types/react @types/react-dom
   ```

3. **Port conflicts**: Change the port in server/index.ts if needed
   ```typescript
   // Change this line in server/index.ts
   const port = process.env.PORT || 3000; // Change from 5000 to another port
   ```

4. **Environment variables**: Create a .env file if needed
   ```
   SESSION_SECRET=your_session_secret_here
   ```

## Project Structure Overview
- `/client` - Frontend React application
- `/server` - Backend Express server
- `/shared` - Shared types and schemas